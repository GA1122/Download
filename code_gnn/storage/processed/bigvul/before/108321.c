unsigned long long Timing::connectStart() const
{
    ResourceLoadTiming* timing = resourceLoadTiming();
    if (!timing)
        return 0;

    int connectStart = timing->connectStart;
    if (connectStart < 0)
        return domainLookupEnd();

    return resourceLoadTimeRelativeToAbsolute(connectStart);
}
