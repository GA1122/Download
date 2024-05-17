void WebPagePrivate::didChangeSettings(WebSettings* webSettings)
{
    Settings* coreSettings = m_page->settings();
    m_page->setGroupName(webSettings->pageGroupName());
    coreSettings->setXSSAuditorEnabled(webSettings->xssAuditorEnabled());
    coreSettings->setLoadsImagesAutomatically(webSettings->loadsImagesAutomatically());
    coreSettings->setShouldDrawBorderWhileLoadingImages(webSettings->shouldDrawBorderWhileLoadingImages());
    coreSettings->setScriptEnabled(webSettings->isJavaScriptEnabled());
    coreSettings->setPrivateBrowsingEnabled(webSettings->isPrivateBrowsingEnabled());
    coreSettings->setDeviceSupportsMouse(webSettings->deviceSupportsMouse());
    coreSettings->setDefaultFixedFontSize(webSettings->defaultFixedFontSize());
    coreSettings->setDefaultFontSize(webSettings->defaultFontSize());
    coreSettings->setMinimumLogicalFontSize(webSettings->minimumFontSize());
    if (!webSettings->serifFontFamily().empty())
        coreSettings->setSerifFontFamily(String(webSettings->serifFontFamily()));
    if (!webSettings->fixedFontFamily().empty())
        coreSettings->setFixedFontFamily(String(webSettings->fixedFontFamily()));
    if (!webSettings->sansSerifFontFamily().empty())
        coreSettings->setSansSerifFontFamily(String(webSettings->sansSerifFontFamily()));
    if (!webSettings->standardFontFamily().empty())
        coreSettings->setStandardFontFamily(String(webSettings->standardFontFamily()));
    coreSettings->setJavaScriptCanOpenWindowsAutomatically(webSettings->canJavaScriptOpenWindowsAutomatically());
    coreSettings->setAllowScriptsToCloseWindows(webSettings->canJavaScriptOpenWindowsAutomatically());  
    coreSettings->setPluginsEnabled(webSettings->arePluginsEnabled());
    coreSettings->setDefaultTextEncodingName(webSettings->defaultTextEncodingName());
    coreSettings->setDownloadableBinaryFontsEnabled(webSettings->downloadableBinaryFontsEnabled());
    coreSettings->setSpatialNavigationEnabled(m_webSettings->isSpatialNavigationEnabled());
    coreSettings->setAsynchronousSpellCheckingEnabled(m_webSettings->isAsynchronousSpellCheckingEnabled());

    BlackBerry::Platform::String stylesheetURL = webSettings->userStyleSheetLocation();
    if (!stylesheetURL.empty())
        coreSettings->setUserStyleSheetLocation(KURL(KURL(), stylesheetURL));

    coreSettings->setFirstScheduledLayoutDelay(webSettings->firstScheduledLayoutDelay());
    coreSettings->setUseCache(webSettings->useWebKitCache());
    coreSettings->setCookieEnabled(webSettings->areCookiesEnabled());

#if ENABLE(SQL_DATABASE)
    static bool dbinit = false;
    if (!dbinit && !webSettings->databasePath().empty()) {
        dbinit = true;
        DatabaseManager::initialize(webSettings->databasePath());
    }

    static bool acinit = false;
    if (!acinit && !webSettings->appCachePath().empty()) {
        acinit = true;
        cacheStorage().setCacheDirectory(webSettings->appCachePath());
    }

    coreSettings->setLocalStorageDatabasePath(webSettings->localStoragePath());
    Database::setIsAvailable(webSettings->isDatabasesEnabled());
    DatabaseSync::setIsAvailable(webSettings->isDatabasesEnabled());

    coreSettings->setLocalStorageEnabled(webSettings->isLocalStorageEnabled());
    coreSettings->setOfflineWebApplicationCacheEnabled(webSettings->isAppCacheEnabled());

    m_page->group().groupSettings()->setLocalStorageQuotaBytes(webSettings->localStorageQuota());
    coreSettings->setSessionStorageQuota(webSettings->sessionStorageQuota());
    coreSettings->setUsesPageCache(webSettings->maximumPagesInCache());
    coreSettings->setFrameFlatteningEnabled(webSettings->isFrameFlatteningEnabled());
#endif

#if ENABLE(INDEXED_DATABASE)
    m_page->group().groupSettings()->setIndexedDBDatabasePath(webSettings->indexedDataBasePath());
#endif


#if ENABLE(WEB_SOCKETS)
    WebSocket::setIsAvailable(webSettings->areWebSocketsEnabled());
#endif

#if ENABLE(FULLSCREEN_API)
    coreSettings->setFullScreenEnabled(true);
#endif

#if ENABLE(VIEWPORT_REFLOW)
    coreSettings->setTextReflowEnabled(webSettings->textReflowMode() == WebSettings::TextReflowEnabled);
#endif

    coreSettings->setShouldUseFirstScheduledLayoutDelay(webSettings->isEmailMode());
    coreSettings->setProcessHTTPEquiv(!webSettings->isEmailMode());

    coreSettings->setShouldUseCrossOriginProtocolCheck(!webSettings->allowCrossSiteRequests());
    coreSettings->setWebSecurityEnabled(!webSettings->allowCrossSiteRequests());
    coreSettings->setApplyPageScaleFactorInCompositor(webSettings->applyDeviceScaleFactorInCompositor());

    cookieManager().setPrivateMode(webSettings->isPrivateBrowsingEnabled());

    CredentialStorage::setPrivateMode(webSettings->isPrivateBrowsingEnabled());

    if (m_mainFrame && m_mainFrame->view()) {
        Color backgroundColor(webSettings->backgroundColor());
        m_mainFrame->view()->updateBackgroundRecursively(backgroundColor, backgroundColor.hasAlpha());

        Platform::userInterfaceThreadMessageClient()->dispatchMessage(
            createMethodCallMessage(&WebPagePrivate::setCompositorBackgroundColor, this, backgroundColor));
    }
    if (m_backingStore) {
        m_backingStore->d->setWebPageBackgroundColor(m_mainFrame && m_mainFrame->view()
            ? m_mainFrame->view()->documentBackgroundColor()
            : webSettings->backgroundColor());
    }

    m_page->setDeviceScaleFactor(webSettings->devicePixelRatio());
}
