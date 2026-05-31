#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <sstream>
#include <iomanip>
#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#endif
#include "DatabaseFacade.h"
#include "Task.h"
#include "Observer.h"

using namespace std;

bool isDateInPast(const string& dateStr) {
    if (dateStr.length() != 10) return true;
    int year, month, day;
    char dash1, dash2;
    istringstream ss(dateStr);
    ss >> year >> dash1 >> month >> dash2 >> day;
    if (ss.fail() || dash1 != '-' || dash2 != '-') return true;
    time_t now = time(nullptr);
    tm* now_tm = localtime(&now);
    int currentYear = now_tm->tm_year + 1900;
    int currentMonth = now_tm->tm_mon + 1;
    int currentDay = now_tm->tm_mday;
    if (year < currentYear) return true;
    if (year == currentYear && month < currentMonth) return true;
    if (year == currentYear && month == currentMonth && day < currentDay) return true;
    return false;
}

DatabaseFacade& db = DatabaseFacade::getInstance();
int currentUserId = -1;
string currentUserLogin;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void checkDeadlines() {
    if (currentUserId == -1) return;
    vector<Task> tasks = db.getTasksForUser(currentUserId);
    if (tasks.empty()) return;
    time_t now = time(nullptr);
    tm* now_tm = localtime(&now);
    char today_str[11];
    strftime(today_str, sizeof(today_str), "%Y-%m-%d", now_tm);
    now += 86400;
    tm* tomorrow_tm = localtime(&now);
    char tomorrow_str[11];
    strftime(tomorrow_str, sizeof(tomorrow_str), "%Y-%m-%d", tomorrow_tm);
    bool hasDeadlines = false;
    for (const auto& task : tasks) {
        string deadline = task.getDeadline();
        if (deadline == today_str || deadline == tomorrow_str) {
            if (!hasDeadlines) {
                cout << "\n=== УВЕДОМЛЕНИЯ О ДЕДЛАЙНАХ ===\n";
                hasDeadlines = true;
            }
            cout << "Задача: " << task.getTitle() << " | Дедлайн: " << deadline << " | Статус: " << task.getStatus() << "\n";
        }
    }
    if (hasDeadlines) cout << "=================================\n\n";
}

void registerUser() {
    string login, password;
    cout << "Логин: "; cin >> login;
    cout << "Пароль: "; cin >> password;
    db.registerUser(login, password);
}

void loginUser() {
    string login, password;
    cout << "Логин: "; cin >> login;
    cout << "Пароль: "; cin >> password;
    if (db.authenticateUser(login, password)) {
        currentUserId = db.getUserIdByLogin(login);
        currentUserLogin = login;
        cout << "Добро пожаловать, " << login << "!\n";
        checkDeadlines();
    }
}

void addTask() {
    if (currentUserId == -1) {
        cout << "Сначала войдите в систему.\n";
        return;
    }
    cin.ignore();
    string title, deadline, description;
    int priority;
    cout << "Название: "; getline(cin, title);
    cout << "Дедлайн (ГГГГ-ММ-ДД): "; getline(cin, deadline);
    if (isDateInPast(deadline)) {
        cout << "Ошибка: дедлайн не может быть в прошлом.\n";
        return;
    }
    cout << "Описание: "; getline(cin, description);
    while (true) {
        cout << "Приоритет (1=Низкий,2=Средний,3=Высокий): ";
        string prioStr;
        getline(cin, prioStr);
        try {
            priority = stoi(prioStr);
            if (priority >= 1 && priority <= 3) break;
            else cout << "Ошибка: введите число от 1 до 3.\n";
        } catch (...) {
            cout << "Ошибка: введите целое число.\n";
        }
    }
    Task task(0, currentUserId, title, deadline, description, priority);
    db.addTask(task);
}

void showTasks() {
    if (currentUserId == -1) {
        cout << "Сначала войдите в систему.\n";
        return;
    }
    vector<Task> tasks = db.getTasksForUser(currentUserId);
    if (tasks.empty()) {
        cout << "Нет задач.\n";
    } else {
        for (const auto& t : tasks) {
            cout << "ID: " << t.getId() << " | " << t.getTitle()
                 << " | Дедлайн: " << t.getDeadline()
                 << " | Статус: " << t.getStatus()
                 << " | Приоритет: " << t.getPriority() << "\n";
            if (!t.getDescription().empty())
                cout << "   Описание: " << t.getDescription() << "\n";
        }
    }
    db.getStatistics(currentUserId); // статистика по запросу
}

void startTimer() {
    if (currentUserId == -1) {
        cout << "Сначала войдите в систему.\n";
        return;
    }
    int taskId;
    while (true) {
        cout << "ID задачи: ";
        string idStr;
        cin >> idStr;
        try {
            taskId = stoi(idStr);
            break;
        } catch (...) {
            cout << "Ошибка: введите целое число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    string startTime(buf);
    db.startTimer(taskId, startTime);
}

void stopTimer() {
    if (currentUserId == -1) {
        cout << "Сначала войдите в систему.\n";
        return;
    }
    int taskId;
    while (true) {
        cout << "ID задачи: ";
        string idStr;
        cin >> idStr;
        try {
            taskId = stoi(idStr);
            break;
        } catch (...) {
            cout << "Ошибка: введите целое число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    string endTime(buf);
    db.stopTimer(taskId, endTime, 0);
}

void generateReport() {
    if (currentUserId == -1) {
        cout << "Сначала войдите в систему.\n";
        return;
    }
    db.generateReport(currentUserId);
}

void editTask() {
    if (currentUserId == -1) {
        cout << "Сначала войдите в систему.\n";
        return;
    }
    int taskId;
    cout << "ID задачи для редактирования: "; cin >> taskId;
    cin.ignore();
    string title, deadline, status, description, priorityStr;
    cout << "Новое название (Enter - оставить без изменений): "; getline(cin, title);
    cout << "Новый дедлайн (ГГГГ-ММ-ДД, Enter - оставить): "; getline(cin, deadline);
    if (!deadline.empty() && isDateInPast(deadline)) {
        cout << "Ошибка: дедлайн не может быть в прошлом.\n";
        return;
    }
    cout << "Новый статус (Pending/InProgress/Completed, Enter - оставить): "; getline(cin, status);
    cout << "Новое описание (Enter - оставить): "; getline(cin, description);
    cout << "Новый приоритет (1=Низкий,2=Средний,3=Высокий, Enter - оставить): ";
    getline(cin, priorityStr);
    int priority = -1;
    if (!priorityStr.empty()) {
        try {
            priority = stoi(priorityStr);
            if (priority < 1 || priority > 3) {
                cout << "Ошибка: приоритет должен быть от 1 до 3. Изменение отменено.\n";
                return;
            }
        } catch (...) {
            cout << "Ошибка: введите целое число. Изменение отменено.\n";
            return;
        }
    }
    db.editTask(taskId, title, deadline, status, description, priority);
}

void deleteTask() {
    if (currentUserId == -1) {
        cout << "Сначала войдите в систему.\n";
        return;
    }
    int taskId;
    cout << "ID задачи для удаления: "; cin >> taskId;
    cout << "Подтвердите удаление (y/n): "; char c; cin >> c;
    if (c == 'y' || c == 'Y')
        db.deleteTask(taskId);
    else
        cout << "Отменено.\n";
}

void showStatistics() {
    if (currentUserId == -1) {
        cout << "Сначала войдите в систему.\n";
        return;
    }
    db.getStatistics(currentUserId);
}

class NotificationObserver : public Observer {
public:
    void update(const std::string& event, int taskId, const std::string& message) override {
        if (event == "task_added") {
            cout << "[Уведомление] Добавлена задача: " << message << " (ID=" << taskId << ")\n";
        } else if (event == "task_edited") {
            cout << "[Уведомление] Задача ID=" << taskId << " отредактирована\n";
        } else if (event == "task_deleted") {
            cout << "[Уведомление] Задача ID=" << taskId << " удалена\n";
        } else if (event == "status_changed") {
            cout << "[Уведомление] Статус задачи ID=" << taskId << " изменён на " << message << "\n";
        } else if (event == "timer_stopped") {
            cout << "[Уведомление] Таймер задачи ID=" << taskId << " остановлен в " << message << "\n";
        }
    }
};

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    auto notifObs = std::make_shared<NotificationObserver>();
    db.attach(notifObs);

    int choice;
    while (true) {
        cout << "\n=== TimeTracker ===\n";
        cout << "1. Регистрация\n2. Вход\n3. Добавить задачу\n4. Показать задачи\n";
        cout << "5. Запустить таймер\n6. Остановить таймер\n7. Отчёт\n";
        cout << "8. Редактировать задачу\n9. Удалить задачу\n10. Статистика\n0. Выход\n";
        cout << "Выбор: ";
        cin >> choice;
        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: addTask(); break;
            case 4: showTasks(); break;
            case 5: startTimer(); break;
            case 6: stopTimer(); break;
            case 7: generateReport(); break;
            case 8: editTask(); break;
            case 9: deleteTask(); break;
            case 10: showStatistics(); break;
            case 0: return 0;
            default: cout << "Неверный выбор.\n"; clearInput();
        }
    }
}