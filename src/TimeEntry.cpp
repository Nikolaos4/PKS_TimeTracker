#include "TimeEntry.h"
#include <iostream>

TimeEntry::TimeEntry(int id, int taskId, const std::string& startTime)
    : id(id), taskId(taskId), startTime(startTime), durationSeconds(0) {}

void TimeEntry::stop(const std::string& endTime, int duration) {
    this->endTime = endTime;
    this->durationSeconds = duration;
    std::cout << "TimeEntry: задача " << taskId << " завершена, длительность " << duration << " сек." << std::endl;
}

int TimeEntry::getDuration() const { return durationSeconds; }