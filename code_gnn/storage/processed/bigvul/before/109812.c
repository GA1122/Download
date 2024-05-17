void Document::setDomain(const String& newDomain, ExceptionState& es)
{
    if (SchemeRegistry::isDomainRelaxationForbiddenForURLScheme(securityOrigin()->protocol())) {
        es.throwSecurityError(ExceptionMessages::failedToSet("domain", "Document", "assignment is forbidden for the '" + securityOrigin()->protocol() + "' scheme."));
        return;
    }


    if (equalIgnoringCase(domain(), newDomain)) {
        securityOrigin()->setDomainFromDOM(newDomain);
        if (m_frame)
            m_frame->script()->updateSecurityOrigin();
        return;
    }

    int oldLength = domain().length();
    int newLength = newDomain.length();
    String exceptionMessage =  ExceptionMessages::failedToSet("domain", "Document", "'" + newDomain + "' is not a suffix of '" + domain() + "'.");
    if (newLength >= oldLength) {
        es.throwSecurityError(exceptionMessage);
        return;
    }

    String test = domain();
    if (test[oldLength - newLength - 1] != '.') {
        es.throwSecurityError(exceptionMessage);
        return;
    }

    test.remove(0, oldLength - newLength);
    if (test != newDomain) {
        es.throwSecurityError(exceptionMessage);
        return;
    }

    securityOrigin()->setDomainFromDOM(newDomain);
    if (m_frame)
        m_frame->script()->updateSecurityOrigin();
}
