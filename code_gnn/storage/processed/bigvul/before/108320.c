unsigned long long Timing::connectEnd() const
{
    ResourceLoadTiming* timing = resourceLoadTiming();
    if (!timing)
        return 0;

    int connectEnd = timing->connectEnd;
    if (connectEnd < 0)
        return connectStart();

    return resourceLoadTimeRelativeToAbsolute(connectEnd);
}
