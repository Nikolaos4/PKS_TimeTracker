#include "TimeEntry.h"
#include <iostream>

TimeEntry::TimeEntry(int id, int taskId, const std::string& startTime)
    : id(id), taskId(taskId), startTime(startTime), durationSeconds(0) {}

int TimeEntry::getDuration() const { return durationSeconds; }

// Метод stop удалён, так как остановка таймера выполняется через DatabaseFacade