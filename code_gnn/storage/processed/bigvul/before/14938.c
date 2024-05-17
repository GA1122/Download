EnableLimitedSchedulingLatency(void)
{
    ++SmartScheduleLatencyLimited;
    SmartScheduleSlice = SmartScheduleInterval;
}
