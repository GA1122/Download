void Document::setDomain(const String& newDomain, ExceptionState& exceptionState)
{
    UseCounter::count(*this, UseCounter::DocumentSetDomain);

    if (isSandboxed(SandboxDocumentDomain)) {
        exceptionState.throwSecurityError("Assignment is forbidden for sandboxed iframes.");
        return;
    }

    if (SchemeRegistry::isDomainRelaxationForbiddenForURLScheme(getSecurityOrigin()->protocol())) {
        exceptionState.throwSecurityError("Assignment is forbidden for the '" + getSecurityOrigin()->protocol() + "' scheme.");
        return;
    }

    if (newDomain.isEmpty()) {
        exceptionState.throwSecurityError("'" + newDomain + "' is an empty domain.");
        return;
    }

    OriginAccessEntry accessEntry(getSecurityOrigin()->protocol(), newDomain, OriginAccessEntry::AllowSubdomains);
    OriginAccessEntry::MatchResult result = accessEntry.matchesOrigin(*getSecurityOrigin());
    if (result == OriginAccessEntry::DoesNotMatchOrigin) {
        exceptionState.throwSecurityError("'" + newDomain + "' is not a suffix of '" + domain() + "'.");
        return;
    }

    if (result == OriginAccessEntry::MatchesOriginButIsPublicSuffix) {
        exceptionState.throwSecurityError("'" + newDomain + "' is a top-level domain.");
        return;
    }

    getSecurityOrigin()->setDomainFromDOM(newDomain);
    if (m_frame)
        m_frame->script().updateSecurityOrigin(getSecurityOrigin());
}
