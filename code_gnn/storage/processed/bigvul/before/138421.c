const KURL& Document::firstPartyForCookies() const
{
    OriginAccessEntry accessEntry(topDocument().url().protocol(), topDocument().url().host(), OriginAccessEntry::AllowSubdomains);
    const Document* currentDocument = this;
    while (currentDocument) {
        while (currentDocument->isSrcdocDocument())
            currentDocument = currentDocument->parentDocument();
        ASSERT(currentDocument);

        if (accessEntry.matchesOrigin(*currentDocument->securityOrigin()) == OriginAccessEntry::DoesNotMatchOrigin)
            return SecurityOrigin::urlWithUniqueSecurityOrigin();

        currentDocument = currentDocument->parentDocument();
    }

    return topDocument().url();
}
