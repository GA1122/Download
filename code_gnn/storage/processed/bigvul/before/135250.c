const KURL& Document::firstPartyForCookies() const
{
    if (SchemeRegistry::shouldTreatURLSchemeAsFirstPartyWhenTopLevel(topDocument().url().protocol()))
        return topDocument().url();

    const OriginAccessEntry& accessEntry = topDocument().accessEntryFromURL();
    const Document* currentDocument = this;
    while (currentDocument) {
        while (currentDocument->isSrcdocDocument())
            currentDocument = currentDocument->parentDocument();
        ASSERT(currentDocument);

        if (accessEntry.matchesDomain(*currentDocument->securityOrigin()) == OriginAccessEntry::DoesNotMatchOrigin)
            return SecurityOrigin::urlWithUniqueSecurityOrigin();

        currentDocument = currentDocument->parentDocument();
    }

    return topDocument().url();
}
