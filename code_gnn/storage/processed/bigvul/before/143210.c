void Document::initSecurityContext(const DocumentInit& initializer)
{
    DCHECK(!getSecurityOrigin());

    if (!initializer.hasSecurityContext()) {
        m_cookieURL = KURL(ParsedURLString, emptyString());
        setSecurityOrigin(SecurityOrigin::createUnique());
        initContentSecurityPolicy();
        return;
    }

    enforceSandboxFlags(initializer.getSandboxFlags());
    setInsecureRequestPolicy(initializer.getInsecureRequestPolicy());
    if (initializer.insecureNavigationsToUpgrade()) {
        for (auto toUpgrade : *initializer.insecureNavigationsToUpgrade())
            addInsecureNavigationUpgrade(toUpgrade);
    }

    if (isSandboxed(SandboxOrigin)) {
        m_cookieURL = m_url;
        setSecurityOrigin(SecurityOrigin::createUnique());
        if (initializer.owner() && initializer.owner()->getSecurityOrigin()->isPotentiallyTrustworthy())
            getSecurityOrigin()->setUniqueOriginIsPotentiallyTrustworthy(true);
        if (initializer.owner() && initializer.owner()->getSecurityOrigin()->canLoadLocalResources())
            getSecurityOrigin()->grantLoadLocalResources();
    } else if (initializer.owner()) {
        m_cookieURL = initializer.owner()->cookieURL();
        setSecurityOrigin(initializer.owner()->getSecurityOrigin());
    } else {
        m_cookieURL = m_url;
        setSecurityOrigin(SecurityOrigin::create(m_url));
    }

    if (initializer.isHostedInReservedIPRange()) {
        setAddressSpace(getSecurityOrigin()->isLocalhost() ? WebAddressSpaceLocal : WebAddressSpacePrivate);
    } else if (getSecurityOrigin()->isLocal()) {
        setAddressSpace(WebAddressSpaceLocal);
    } else {
        setAddressSpace(WebAddressSpacePublic);
    }

    if (importsController()) {
        setContentSecurityPolicy(importsController()->master()->contentSecurityPolicy());
    } else {
        initContentSecurityPolicy();
    }

    if (getSecurityOrigin()->hasSuborigin())
        enforceSuborigin(*getSecurityOrigin()->suborigin());

    if (Settings* settings = initializer.settings()) {
        if (!settings->webSecurityEnabled()) {
            getSecurityOrigin()->grantUniversalAccess();
        } else if (getSecurityOrigin()->isLocal()) {
            if (settings->allowUniversalAccessFromFileURLs()) {
                getSecurityOrigin()->grantUniversalAccess();
            } else if (!settings->allowFileAccessFromFileURLs()) {
                getSecurityOrigin()->blockLocalAccessFromLocalOrigin();
            }
        }
    }

    if (initializer.shouldTreatURLAsSrcdocDocument()) {
        m_isSrcdocDocument = true;
        setBaseURLOverride(initializer.parentBaseURL());
    }

    if (getSecurityOrigin()->isUnique() && SecurityOrigin::create(m_url)->isPotentiallyTrustworthy())
        getSecurityOrigin()->setUniqueOriginIsPotentiallyTrustworthy(true);

    if (getSecurityOrigin()->hasSuborigin())
        enforceSuborigin(*getSecurityOrigin()->suborigin());
}
