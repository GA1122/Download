void Document::initSecurityContext(const DocumentInit& initializer)
{
    if (haveInitializedSecurityOrigin()) {
        ASSERT(securityOrigin());
        return;
    }

    if (initializer.isHostedInReservedIPRange())
        setHostedInReservedIPRange();

    if (!initializer.hasSecurityContext()) {
        m_cookieURL = KURL(ParsedURLString, emptyString());
        setSecurityOrigin(SecurityOrigin::createUnique());
        initContentSecurityPolicy();
        return;
    }

    m_cookieURL = m_url;
    enforceSandboxFlags(initializer.sandboxFlags());
    if (initializer.shouldEnforceStrictMixedContentChecking())
        enforceStrictMixedContentChecking();
    setInsecureRequestsPolicy(initializer.insecureRequestsPolicy());
    if (initializer.insecureNavigationsToUpgrade()) {
        for (auto toUpgrade : *initializer.insecureNavigationsToUpgrade())
            addInsecureNavigationUpgrade(toUpgrade);
    }
    setSecurityOrigin(isSandboxed(SandboxOrigin) ? SecurityOrigin::createUnique() : SecurityOrigin::create(m_url));

    if (importsController()) {
        setContentSecurityPolicy(importsController()->master()->contentSecurityPolicy());
    } else {
        initContentSecurityPolicy();
    }

    if (Settings* settings = initializer.settings()) {
        if (!settings->webSecurityEnabled()) {
            securityOrigin()->grantUniversalAccess();
        } else if (securityOrigin()->isLocal()) {
            if (settings->allowUniversalAccessFromFileURLs()) {
                securityOrigin()->grantUniversalAccess();
            } else if (!settings->allowFileAccessFromFileURLs()) {
                securityOrigin()->blockLocalAccessFromLocalOrigin();
            }
        }
    }

    if (initializer.shouldTreatURLAsSrcdocDocument()) {
        m_isSrcdocDocument = true;
        setBaseURLOverride(initializer.parentBaseURL());
    }

    if (!shouldInheritSecurityOriginFromOwner(m_url))
        return;


    if (!initializer.owner()) {
        didFailToInitializeSecurityOrigin();
        return;
    }

    if (isSandboxed(SandboxOrigin)) {
        if (initializer.owner()->securityOrigin()->canLoadLocalResources())
            securityOrigin()->grantLoadLocalResources();
        return;
    }

    m_cookieURL = initializer.owner()->cookieURL();
    setSecurityOrigin(initializer.owner()->securityOrigin());
}
