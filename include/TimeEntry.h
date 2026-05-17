#ifndef TIMEENTRY_H
#define TIMEENTRY_H

#include <string>

class TimeEntry {
private:
    int id;
    int taskId;
    std::string startTime;
    std::string endTime;
    int durationSeconds;
public:
    TimeEntry(int id, int taskId, const std::string& startTime);
    void stop(const std::string& endTime, int duration);
    int getDuration() const;
};

#endif