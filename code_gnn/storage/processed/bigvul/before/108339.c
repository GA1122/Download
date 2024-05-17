unsigned long long Timing::responseStart() const
{
    ResourceLoadTiming* timing = resourceLoadTiming();
    if (!timing)
        return 0;

    ASSERT(timing->receiveHeadersEnd >= 0);
    return resourceLoadTimeRelativeToAbsolute(timing->receiveHeadersEnd);
}
