bool Document::isSecureContext(String& errorMessage, const SecureContextCheck privilegeContextCheck) const
{
    if (SecurityContext::isSandboxed(SandboxOrigin)) {
        if (!SecurityOrigin::create(url())->isPotentiallyTrustworthy(errorMessage))
            return false;
    } else {
        if (!securityOrigin()->isPotentiallyTrustworthy(errorMessage))
            return false;
    }

    if (privilegeContextCheck == StandardSecureContextCheck) {
        Document* context = parentDocument();
        while (context) {
            if (!context->isSrcdocDocument()) {
                if (context->securityContext().isSandboxed(SandboxOrigin)) {
                    RefPtr<SecurityOrigin> origin = SecurityOrigin::create(context->url());
                    if (!origin->isPotentiallyTrustworthy(errorMessage))
                        return false;
                } else {
                    if (!context->securityOrigin()->isPotentiallyTrustworthy(errorMessage))
                        return false;
                }
            }
            context = context->parentDocument();
        }
    }
    return true;
}
