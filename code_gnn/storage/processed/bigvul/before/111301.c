void WebPage::removeOriginAccessWhitelistEntry(const BlackBerry::Platform::String& sourceOrigin, const BlackBerry::Platform::String& destinationOrigin, bool allowDestinationSubdomains)
{
    d->removeOriginAccessWhitelistEntry(sourceOrigin, destinationOrigin, allowDestinationSubdomains);
}
