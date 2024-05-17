bool Document::isSecureContextImpl(String* errorMessage, const SecureContextCheck privilegeContextCheck) const
{
    if (SecurityContext::isSandboxed(SandboxOrigin)) {
        RefPtr<SecurityOrigin> origin = SecurityOrigin::create(url());
        if (!isOriginPotentiallyTrustworthy(origin.get(), errorMessage))
            return false;
        if (SchemeRegistry::schemeShouldBypassSecureContextCheck(origin->protocol()))
            return true;
    } else {
        if (!isOriginPotentiallyTrustworthy(securityOrigin(), errorMessage))
            return false;
        if (SchemeRegistry::schemeShouldBypassSecureContextCheck(securityOrigin()->protocol()))
            return true;
    }

    if (privilegeContextCheck == StandardSecureContextCheck) {
        Document* context = parentDocument();
        while (context) {
            if (!context->isSrcdocDocument()) {
                if (context->securityContext().isSandboxed(SandboxOrigin)) {
                    RefPtr<SecurityOrigin> origin = SecurityOrigin::create(context->url());
                    if (!isOriginPotentiallyTrustworthy(origin.get(), errorMessage))
                        return false;
                } else {
                    if (!isOriginPotentiallyTrustworthy(context->securityOrigin(), errorMessage))
                        return false;
                }
            }
            context = context->parentDocument();
        }
    }
    return true;
}
