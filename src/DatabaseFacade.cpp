#include "DatabaseFacade.h"
#include <pqxx/pqxx>
#include <iostream>
#include <stdexcept>
#include <ctime>
#include "hash_utils.h"

static DatabaseFacade* instance = nullptr;

DatabaseFacade::DatabaseFacade() {
    try {
        conn = new pqxx::connection("dbname=timetracker user=postgres password=hello1209 host=localhost port=5432");
        if (!conn->is_open()) {
            throw std::runtime_error("Невозможно подключиться к базе данных");
        }
    } catch (const std::exception&) {
        std::cerr << "Ошибка подключения к базе данных" << std::endl;
        throw;
    }
}

DatabaseFacade::~DatabaseFacade() {
    delete conn;
}

DatabaseFacade& DatabaseFacade::getInstance() {
    if (!instance) {
        instance = new DatabaseFacade();
    }
    return *instance;
}

bool DatabaseFacade::registerUser(const std::string& login, const std::string& password) {
    try {
        pqxx::work txn(*conn);
        std::string hashed = hashPassword(password);
        txn.exec_params("INSERT INTO users (login, password_hash) VALUES ($1, $2)", login, hashed);
        txn.commit();
        std::cout << "Пользователь зарегистрирован: " << login << std::endl;
        return true;
    } catch (const std::exception&) {
        std::cerr << "Ошибка регистрации (возможно, логин уже существует)" << std::endl;
        return false;
    }
}

bool DatabaseFacade::authenticateUser(const std::string& login, const std::string& password) {
    try {
        pqxx::work txn(*conn);
        auto res = txn.exec_params("SELECT password_hash FROM users WHERE login = $1", login);
        if (res.empty()) return false;
        std::string storedHash = res[0][0].as<std::string>();
        return storedHash == hashPassword(password);
    } catch (const std::exception&) {
        std::cerr << "Ошибка аутентификации" << std::endl;
        return false;
    }
}

bool DatabaseFacade::addTask(const Task& task) {
    try {
        pqxx::work txn(*conn);
        txn.exec_params(
            "INSERT INTO tasks (user_id, title, deadline, status, description, priority) VALUES ($1, $2, $3, $4, $5, $6)",
            task.getUserId(), task.getTitle(), task.getDeadline(), task.getStatus(),
            task.getDescription(), task.getPriority());
        txn.commit();
        std::cout << "Задача добавлена: " << task.getTitle() << std::endl;
        notify("task_added", task.getId(), task.getTitle());
        return true;
    } catch (const std::exception&) {
        std::cerr << "Не удалось добавить задачу" << std::endl;
        return false;
    }
}

std::vector<Task> DatabaseFacade::getTasksForUser(int userId) {
    std::vector<Task> tasks;
    try {
        pqxx::work txn(*conn);
        auto res = txn.exec_params(
            "SELECT id, user_id, title, deadline, status, description, priority "
            "FROM tasks WHERE user_id = $1 ORDER BY deadline ASC, priority DESC",
            userId);
        for (const auto& row : res) {
            Task t(row[0].as<int>(), row[1].as<int>(), row[2].as<std::string>(),
                   row[3].as<std::string>(), row[5].as<std::string>(), row[6].as<int>());
            t.setStatus(row[4].as<std::string>());
            tasks.push_back(t);
        }
    } catch (const std::exception&) {
        std::cerr << "Ошибка получения списка задач" << std::endl;
    }
    return tasks;
}

bool DatabaseFacade::updateTaskStatus(int taskId, const std::string& status) {
    try {
        pqxx::work txn(*conn);
        txn.exec_params("UPDATE tasks SET status = $1 WHERE id = $2", status, taskId);
        txn.commit();
        notify("status_changed", taskId, status);
        return true;
    } catch (const std::exception&) {
        std::cerr << "Ошибка обновления статуса задачи" << std::endl;
        return false;
    }
}

bool DatabaseFacade::startTimer(int taskId, const std::string& startTime) {
    try {
        pqxx::work txn(*conn);
        txn.exec_params("INSERT INTO time_entries (task_id, start_time) VALUES ($1, $2)", taskId, startTime);
        txn.commit();
        std::cout << "Таймер запущен для задачи " << taskId << std::endl;
        return true;
    } catch (const std::exception&) {
        std::cerr << "Ошибка запуска таймера" << std::endl;
        return false;
    }
}

bool DatabaseFacade::stopTimer(int taskId, const std::string& endTime, int /*durationIgnored*/) {
    try {
        pqxx::work txn(*conn);
        txn.exec_params(
            "UPDATE time_entries SET end_time = $1, duration_seconds = EXTRACT(EPOCH FROM ($1::timestamp - start_time)) "
            "WHERE task_id = $2 AND end_time IS NULL",
            endTime, taskId);
        txn.commit();
        std::cout << "Таймер остановлен для задачи " << taskId << std::endl;
        notify("timer_stopped", taskId, endTime);
        return true;
    } catch (const std::exception&) {
        std::cerr << "Ошибка остановки таймера" << std::endl;
        return false;
    }
}

bool DatabaseFacade::addNotification(const Notification& notif) {
    try {
        pqxx::work txn(*conn);
        txn.exec_params("INSERT INTO notifications (user_id, task_id, message) VALUES ($1, $2, $3)",
                        notif.getUserId(), notif.getTaskId(), notif.getMessage());
        txn.commit();
        std::cout << "Уведомление добавлено" << std::endl;
        return true;
    } catch (const std::exception&) {
        std::cerr << "Ошибка добавления уведомления" << std::endl;
        return false;
    }
}

bool DatabaseFacade::generateReport(int userId) {
    try {
        pqxx::work txn(*conn);
        auto res = txn.exec_params(
            "SELECT t.title, te.start_time, te.end_time, te.duration_seconds "
            "FROM tasks t LEFT JOIN time_entries te ON t.id = te.task_id "
            "WHERE t.user_id = $1", userId);
        std::cout << "Отчёт для пользователя " << userId << ":\n";
        bool hasData = false;
        for (const auto& row : res) {
            hasData = true;
            std::cout << "Задача: " << row[0].as<std::string>()
                      << " | старт: " << row[1].as<std::string>()
                      << " | конец: " << row[2].as<std::string>()
                      << " | длительность: " << row[3].as<int>() << " сек\n";
        }
        if (!hasData) std::cout << "Нет данных для отчёта.\n";
        return true;
    } catch (const std::exception&) {
        std::cerr << "Ошибка формирования отчёта" << std::endl;
        return false;
    }
}

int DatabaseFacade::getUserIdByLogin(const std::string& login) {
    try {
        pqxx::work txn(*conn);
        auto res = txn.exec_params("SELECT id FROM users WHERE login = $1", login);
        if (res.empty()) return -1;
        return res[0][0].as<int>();
    } catch (const std::exception&) {
        std::cerr << "Ошибка получения ID пользователя" << std::endl;
        return -1;
    }
}

bool DatabaseFacade::editTask(int taskId, const std::string& newTitle, const std::string& newDeadline,
                              const std::string& newStatus, const std::string& newDescription, int newPriority) {
    try {
        pqxx::work txn(*conn);
        txn.exec_params(
            "UPDATE tasks SET "
            "title = CASE WHEN $1 = '' THEN title ELSE $1 END, "
            "deadline = CASE WHEN $2 = '' THEN deadline ELSE $2::DATE END, "
            "status = CASE WHEN $3 = '' THEN status ELSE $3 END, "
            "description = CASE WHEN $4 = '' THEN description ELSE $4 END, "
            "priority = CASE WHEN $5 = -1 THEN priority ELSE $5 END "
            "WHERE id = $6",
            newTitle, newDeadline, newStatus, newDescription, newPriority, taskId);
        txn.commit();
        std::cout << "Задача обновлена" << std::endl;
        notify("task_edited", taskId, newTitle);
        return true;
    } catch (const std::exception&) {
        std::cerr << "Ошибка редактирования задачи" << std::endl;
        return false;
    }
}

bool DatabaseFacade::deleteTask(int taskId) {
    try {
        pqxx::work txn(*conn);
        txn.exec_params("DELETE FROM tasks WHERE id = $1", taskId);
        txn.commit();
        std::cout << "Задача удалена" << std::endl;
        notify("task_deleted", taskId, "");
        return true;
    } catch (const std::exception&) {
        std::cerr << "Ошибка удаления задачи" << std::endl;
        return false;
    }
}

void DatabaseFacade::getStatistics(int userId) {
    try {
        pqxx::work txn(*conn);
        auto totalRes = txn.exec_params("SELECT COUNT(*) FROM tasks WHERE user_id = $1", userId);
        int total = totalRes[0][0].as<int>();
        auto completedRes = txn.exec_params("SELECT COUNT(*) FROM tasks WHERE user_id = $1 AND status = 'Completed'", userId);
        int completed = completedRes[0][0].as<int>();
        time_t now = time(nullptr);
        tm* now_tm = localtime(&now);
        char today[11];
        strftime(today, sizeof(today), "%Y-%m-%d", now_tm);
        auto overdueRes = txn.exec_params(
            "SELECT COUNT(*) FROM tasks WHERE user_id = $1 AND deadline < $2 AND status != 'Completed'",
            userId, today);
        int overdue = overdueRes[0][0].as<int>();
        auto timeRes = txn.exec_params(
            "SELECT COALESCE(SUM(te.duration_seconds), 0) FROM time_entries te "
            "JOIN tasks t ON te.task_id = t.id WHERE t.user_id = $1", userId);
        int totalSeconds = timeRes[0][0].as<int>();
        double percent = (total == 0) ? 0.0 : (100.0 * completed / total);

        std::cout << "\n=== СТАТИСТИКА ===\n";
        std::cout << "Всего задач: " << total << "\n";
        std::cout << "Выполнено: " << completed << " (" << percent << "%)\n";
        std::cout << "Просрочено: " << overdue << "\n";
        std::cout << "Общее время: " << totalSeconds / 3600 << " ч " << (totalSeconds % 3600) / 60 << " мин\n";
        std::cout << "=================\n\n";
    } catch (const std::exception& e) {
        std::cerr << "Ошибка получения статистики: " << e.what() << std::endl;
    }
}