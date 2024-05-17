static double lowResUTCTime()
{
    FILETIME fileTime;
    GetSystemTimeAsFileTime(&fileTime);

    ULARGE_INTEGER dateTime;
    memcpy(&dateTime, &fileTime, sizeof(dateTime));

    static const ULONGLONG epochBias = 116444736000000000ULL;
    static const double hundredsOfNanosecondsPerMillisecond = 10000;
    return (dateTime.QuadPart - epochBias) / hundredsOfNanosecondsPerMillisecond;
}
