double currentTime()
{
    struct timeval now;
    struct timezone zone;

    gettimeofday(&now, &zone);
    return static_cast<double>(now.tv_sec) + (double)(now.tv_usec / 1000000.0);
}
