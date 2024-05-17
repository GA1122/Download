unsigned long long Timing::domainLookupEnd() const
{
    ResourceLoadTiming* timing = resourceLoadTiming();
    if (!timing)
        return 0;

    int dnsEnd = timing->dnsEnd;
    if (dnsEnd < 0)
        return domainLookupStart();

    return resourceLoadTimeRelativeToAbsolute(dnsEnd);
}
