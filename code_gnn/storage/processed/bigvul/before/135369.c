void Document::setDomain(const String& newDomain, ExceptionState& exceptionState)
{
    UseCounter::count(*this, UseCounter::DocumentSetDomain);

    if (isSandboxed(SandboxDocumentDomain)) {
        exceptionState.throwSecurityError("Assignment is forbidden for sandboxed iframes.");
        return;
    }

    if (SchemeRegistry::isDomainRelaxationForbiddenForURLScheme(securityOrigin()->protocol())) {
        exceptionState.throwSecurityError("Assignment is forbidden for the '" + securityOrigin()->protocol() + "' scheme.");
        return;
    }

    if (newDomain.isEmpty()) {
        exceptionState.throwSecurityError("'" + newDomain + "' is an empty domain.");
        return;
    }

    OriginAccessEntry accessEntry(securityOrigin()->protocol(), newDomain, OriginAccessEntry::AllowSubdomains);
    OriginAccessEntry::MatchResult result = accessEntry.matchesOrigin(*securityOrigin());
    if (result == OriginAccessEntry::DoesNotMatchOrigin) {
        exceptionState.throwSecurityError("'" + newDomain + "' is not a suffix of '" + domain() + "'.");
        return;
    }

    if (result == OriginAccessEntry::MatchesOriginButIsPublicSuffix) {
        exceptionState.throwSecurityError("'" + newDomain + "' is a top-level domain.");
        return;
    }

    securityOrigin()->setDomainFromDOM(newDomain);
    if (m_frame)
        m_frame->script().updateSecurityOrigin(securityOrigin());
}
