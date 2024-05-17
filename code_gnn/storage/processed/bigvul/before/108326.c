unsigned long long Timing::domainLookupStart() const
{
    ResourceLoadTiming* timing = resourceLoadTiming();
    if (!timing)
        return 0;

    int dnsStart = timing->dnsStart;
    if (dnsStart < 0)
        return fetchStart();

    return resourceLoadTimeRelativeToAbsolute(dnsStart);
}
