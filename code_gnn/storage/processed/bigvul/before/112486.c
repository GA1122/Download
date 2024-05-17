void Document::initSecurityContext()
{
    if (haveInitializedSecurityOrigin()) {
        ASSERT(securityOrigin());
        return;
    }

    if (!m_frame) {
        m_cookieURL = KURL(ParsedURLString, emptyString());
        setSecurityOrigin(SecurityOrigin::createUnique());
        setContentSecurityPolicy(ContentSecurityPolicy::create(this));
        return;
    }

    m_cookieURL = m_url;
    enforceSandboxFlags(m_frame->loader()->effectiveSandboxFlags());
    setSecurityOrigin(isSandboxed(SandboxOrigin) ? SecurityOrigin::createUnique() : SecurityOrigin::create(m_url));
    setContentSecurityPolicy(ContentSecurityPolicy::create(this));

    if (Settings* settings = this->settings()) {
        if (!settings->webSecurityEnabled()) {
            securityOrigin()->grantUniversalAccess();
        } else if (securityOrigin()->isLocal()) {
            if (settings->allowUniversalAccessFromFileURLs() || m_frame->loader()->client()->shouldForceUniversalAccessFromLocalURL(m_url)) {
                securityOrigin()->grantUniversalAccess();
            } else if (!settings->allowFileAccessFromFileURLs()) {
                securityOrigin()->enforceFilePathSeparation();
            }
        }
        securityOrigin()->setStorageBlockingPolicy(settings->storageBlockingPolicy());
    }

    Document* parentDocument = ownerElement() ? ownerElement()->document() : 0;
    if (parentDocument && m_frame->loader()->shouldTreatURLAsSrcdocDocument(url())) {
        m_isSrcdocDocument = true;
        setBaseURLOverride(parentDocument->baseURL());
    }

    m_mayDisplaySeamlesslyWithParent = isEligibleForSeamless(parentDocument, this);

    if (!shouldInheritSecurityOriginFromOwner(m_url))
        return;


    Frame* ownerFrame = m_frame->tree()->parent();
    if (!ownerFrame)
        ownerFrame = m_frame->loader()->opener();

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
