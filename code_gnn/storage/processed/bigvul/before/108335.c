unsigned long long Timing::requestEnd() const
{
    ResourceLoadTiming* timing = resourceLoadTiming();
    if (!timing)
        return 0;

    ASSERT(timing->sendEnd >= 0);
    return resourceLoadTimeRelativeToAbsolute(timing->sendEnd);
}
