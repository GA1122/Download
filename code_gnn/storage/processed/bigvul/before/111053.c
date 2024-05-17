void WebPagePrivate::addOriginAccessWhitelistEntry(const BlackBerry::Platform::String& sourceOrigin, const BlackBerry::Platform::String& destinationOrigin, bool allowDestinationSubdomains)
{
    RefPtr<SecurityOrigin> source = SecurityOrigin::createFromString(sourceOrigin);
    if (source->isUnique())
        return;

    KURL destination(KURL(), destinationOrigin);
    SecurityPolicy::addOriginAccessWhitelistEntry(*source, destination.protocol(), destination.host(), allowDestinationSubdomains);
}
