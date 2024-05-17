const KURL Document::firstPartyForCookies() const
{
    Frame* top = frame()->tree().top();
    KURL topDocumentURL = top->isLocalFrame()
        ? toLocalFrame(top)->document()->url()
        : KURL(KURL(), top->securityContext()->getSecurityOrigin()->toString());
    if (SchemeRegistry::shouldTreatURLSchemeAsFirstPartyWhenTopLevel(topDocumentURL.protocol()))
        return topDocumentURL;

    const OriginAccessEntry& accessEntry = top->isLocalFrame()
        ? toLocalFrame(top)->document()->accessEntryFromURL()
        : OriginAccessEntry(topDocumentURL.protocol(), topDocumentURL.host(), OriginAccessEntry::AllowRegisterableDomains);
    const Frame* currentFrame = frame();
    while (currentFrame) {
        while (currentFrame->isLocalFrame() && toLocalFrame(currentFrame)->document()->isSrcdocDocument())
            currentFrame = currentFrame->tree().parent();
        DCHECK(currentFrame);

        if (accessEntry.matchesDomain(*currentFrame->securityContext()->getSecurityOrigin()) == OriginAccessEntry::DoesNotMatchOrigin)
            return SecurityOrigin::urlWithUniqueSecurityOrigin();

        currentFrame = currentFrame->tree().parent();
    }

    return topDocumentURL;
}
