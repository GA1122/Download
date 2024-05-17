double currentTime()
{
    GTimeVal now;
    g_get_current_time(&now);
    return static_cast<double>(now.tv_sec) + static_cast<double>(now.tv_usec / 1000000.0);
}
