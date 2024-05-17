bool TestController::resetStateToConsistentValues()
{
    m_state = Resetting;

    m_beforeUnloadReturnValue = true;

    WKRetainPtr<WKStringRef> messageName = adoptWK(WKStringCreateWithUTF8CString("Reset"));
    WKRetainPtr<WKMutableDictionaryRef> resetMessageBody = adoptWK(WKMutableDictionaryCreate());

    WKRetainPtr<WKStringRef> shouldGCKey = adoptWK(WKStringCreateWithUTF8CString("ShouldGC"));
    WKRetainPtr<WKBooleanRef> shouldGCValue = adoptWK(WKBooleanCreate(m_gcBetweenTests));
    WKDictionaryAddItem(resetMessageBody.get(), shouldGCKey.get(), shouldGCValue.get());

    WKContextPostMessageToInjectedBundle(TestController::shared().context(), messageName.get(), resetMessageBody.get());

    WKContextSetShouldUseFontSmoothing(TestController::shared().context(), false);

    WKContextSetCacheModel(TestController::shared().context(), kWKCacheModelDocumentBrowser);


#if PLATFORM(MAC) || PLATFORM(QT) || PLATFORM(GTK) || PLATFORM(EFL)
    m_eventSenderProxy = adoptPtr(new EventSenderProxy(this));
#endif

    WKPreferencesRef preferences = WKPageGroupGetPreferences(m_pageGroup.get());
    WKPreferencesResetTestRunnerOverrides(preferences);
    WKPreferencesSetOfflineWebApplicationCacheEnabled(preferences, true);
    WKPreferencesSetFontSmoothingLevel(preferences, kWKFontSmoothingLevelNoSubpixelAntiAliasing);
    WKPreferencesSetXSSAuditorEnabled(preferences, false);
    WKPreferencesSetWebAudioEnabled(preferences, true);
    WKPreferencesSetDeveloperExtrasEnabled(preferences, true);
    WKPreferencesSetJavaScriptExperimentsEnabled(preferences, true);
    WKPreferencesSetJavaScriptCanOpenWindowsAutomatically(preferences, true);
    WKPreferencesSetJavaScriptCanAccessClipboard(preferences, true);
    WKPreferencesSetDOMPasteAllowed(preferences, true);
    WKPreferencesSetUniversalAccessFromFileURLsAllowed(preferences, true);
    WKPreferencesSetFileAccessFromFileURLsAllowed(preferences, true);
#if ENABLE(FULLSCREEN_API)
    WKPreferencesSetFullScreenEnabled(preferences, true);
#endif
    WKPreferencesSetPageCacheEnabled(preferences, false);
    WKPreferencesSetAsynchronousPluginInitializationEnabled(preferences, false);
    WKPreferencesSetAsynchronousPluginInitializationEnabledForAllPlugins(preferences, false);
    WKPreferencesSetArtificialPluginInitializationDelayEnabled(preferences, false);
    WKPreferencesSetTabToLinksEnabled(preferences, false);
    WKPreferencesSetInteractiveFormValidationEnabled(preferences, true);
    WKPreferencesSetMockScrollbarsEnabled(preferences, true);

#if !PLATFORM(QT)
    static WKStringRef standardFontFamily = WKStringCreateWithUTF8CString("Times");
    static WKStringRef cursiveFontFamily = WKStringCreateWithUTF8CString("Apple Chancery");
    static WKStringRef fantasyFontFamily = WKStringCreateWithUTF8CString("Papyrus");
    static WKStringRef fixedFontFamily = WKStringCreateWithUTF8CString("Courier");
    static WKStringRef pictographFontFamily = WKStringCreateWithUTF8CString("Apple Color Emoji");
    static WKStringRef sansSerifFontFamily = WKStringCreateWithUTF8CString("Helvetica");
    static WKStringRef serifFontFamily = WKStringCreateWithUTF8CString("Times");

    WKPreferencesSetStandardFontFamily(preferences, standardFontFamily);
    WKPreferencesSetCursiveFontFamily(preferences, cursiveFontFamily);
    WKPreferencesSetFantasyFontFamily(preferences, fantasyFontFamily);
    WKPreferencesSetFixedFontFamily(preferences, fixedFontFamily);
    WKPreferencesSetPictographFontFamily(preferences, pictographFontFamily);
    WKPreferencesSetSansSerifFontFamily(preferences, sansSerifFontFamily);
    WKPreferencesSetSerifFontFamily(preferences, serifFontFamily);
#endif
    WKPreferencesSetScreenFontSubstitutionEnabled(preferences, true);
    WKPreferencesSetInspectorUsesWebKitUserInterface(preferences, true);
#if !PLATFORM(MAC)
    WKTextCheckerContinuousSpellCheckingEnabledStateChanged(true);
#endif

    m_mainWebView->removeChromeInputField();
    m_mainWebView->focus();

    WKPageSetCustomBackingScaleFactor(m_mainWebView->page(), 0);

#if PLATFORM(EFL)
    WKRect rect = m_mainWebView->windowFrame();
    m_mainWebView->setWindowFrame(WKRectMake(rect.origin.x, rect.origin.y, 800, 600));
#endif

    m_webNotificationProvider.reset();

    m_geolocationPermissionRequests.clear();
    m_isGeolocationPermissionSet = false;
    m_isGeolocationPermissionAllowed = false;

    setCustomPolicyDelegate(false, false);

    m_workQueueManager.clearWorkQueue();

    m_handlesAuthenticationChallenges = false;
    m_authenticationUsername = String();
    m_authenticationPassword = String();

    m_doneResetting = false;

    WKPageLoadURL(m_mainWebView->page(), blankURL());
    runUntil(m_doneResetting, ShortTimeout);
    return m_doneResetting;
}
