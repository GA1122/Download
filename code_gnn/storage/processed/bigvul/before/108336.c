unsigned long long Timing::requestStart() const
{
    ResourceLoadTiming* timing = resourceLoadTiming();
    if (!timing)
        return 0;

    ASSERT(timing->sendStart >= 0);
    return resourceLoadTimeRelativeToAbsolute(timing->sendStart);
}
