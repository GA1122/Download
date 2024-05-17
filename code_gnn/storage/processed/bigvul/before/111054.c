void WebPage::addOriginAccessWhitelistEntry(const BlackBerry::Platform::String& sourceOrigin, const BlackBerry::Platform::String& destinationOrigin, bool allowDestinationSubdomains)
{
    d->addOriginAccessWhitelistEntry(sourceOrigin, destinationOrigin, allowDestinationSubdomains);
}
