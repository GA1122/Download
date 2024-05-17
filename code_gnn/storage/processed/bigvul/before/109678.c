const KURL& Document::firstPartyForCookies() const
{
    return topDocument()->url();
}
