void Document::initSecurityContext(const DocumentInit& initializer)
{
    if (haveInitializedSecurityOrigin()) {
        ASSERT(securityOrigin());
        return;
    }

    if (!initializer.hasSecurityContext()) {
        m_cookieURL = KURL(ParsedURLString, emptyString());
        setSecurityOrigin(SecurityOrigin::createUnique());
        setContentSecurityPolicy(ContentSecurityPolicy::create(this));
        return;
    }

    m_cookieURL = m_url;
    enforceSandboxFlags(initializer.sandboxFlags());
    setSecurityOrigin(isSandboxed(SandboxOrigin) ? SecurityOrigin::createUnique() : SecurityOrigin::create(m_url));
    setContentSecurityPolicy(ContentSecurityPolicy::create(this));

    if (Settings* settings = initializer.settings()) {
        if (!settings->webSecurityEnabled()) {
            securityOrigin()->grantUniversalAccess();
        } else if (securityOrigin()->isLocal()) {
            if (settings->allowUniversalAccessFromFileURLs()) {
                securityOrigin()->grantUniversalAccess();
            } else if (!settings->allowFileAccessFromFileURLs()) {
                securityOrigin()->enforceFilePathSeparation();
            }
        }
    }

    Document* parentDocument = ownerElement() ? &ownerElement()->document() : 0;
    if (parentDocument && initializer.shouldTreatURLAsSrcdocDocument()) {
        m_isSrcdocDocument = true;
        setBaseURLOverride(parentDocument->baseURL());
    }

    m_mayDisplaySeamlesslyWithParent = isEligibleForSeamless(parentDocument, this);

    if (!shouldInheritSecurityOriginFromOwner(m_url))
        return;


    Frame* ownerFrame = initializer.ownerFrame();
    if (!ownerFrame) {
        didFailToInitializeSecurityOrigin();
        return;
    }

    if (isSandboxed(SandboxOrigin)) {
        if (ownerFrame->document()->securityOrigin()->canLoadLocalResources())
            securityOrigin()->grantLoadLocalResources();
        return;
    }

    m_cookieURL = ownerFrame->document()->cookieURL();
    setSecurityOrigin(ownerFrame->document()->securityOrigin());
}
