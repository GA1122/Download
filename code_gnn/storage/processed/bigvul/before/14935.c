DisableLimitedSchedulingLatency(void)
{
    --SmartScheduleLatencyLimited;

     
    if (SmartScheduleLatencyLimited < 0)
        SmartScheduleLatencyLimited = 0;
}
