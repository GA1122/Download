void TestController::initialize(int argc, const char* argv[])
{
    platformInitialize();

    if (argc < 2) {
        fputs("Usage: WebKitTestRunner [options] filename [filename2..n]\n", stderr);
        exit(1);
    }

    bool printSupportedFeatures = false;

    for (int i = 1; i < argc; ++i) {
        std::string argument(argv[i]);

        if (argument == "--timeout" && i + 1 < argc) {
            m_longTimeout = atoi(argv[++i]);
            m_shortTimeout = defaultShortTimeout * m_longTimeout / defaultLongTimeout;
            continue;
        }

        if (argument == "--no-timeout") {
            m_useWaitToDumpWatchdogTimer = false;
            continue;
        }

        if (argument == "--no-timeout-at-all") {
            m_useWaitToDumpWatchdogTimer = false;
            m_forceNoTimeout = true;
            continue;
        }

        if (argument == "--verbose") {
            m_verbose = true;
            continue;
        }
        if (argument == "--gc-between-tests") {
            m_gcBetweenTests = true;
            continue;
        }
        if (argument == "--pixel-tests" || argument == "-p") {
            m_shouldDumpPixelsForAllTests = true;
            continue;
        }
        if (argument == "--print-supported-features") {
            printSupportedFeatures = true;
            break;
        }


        if (argument.length() >= 2 && argument[0] == '-' && argument[1] == '-')
            continue;

        m_paths.push_back(argument);
    }

    if (printSupportedFeatures) {
        exit(0);
    }

    m_usingServerMode = (m_paths.size() == 1 && m_paths[0] == "-");
    if (m_usingServerMode)
        m_printSeparators = true;
    else
        m_printSeparators = m_paths.size() > 1;

    initializeInjectedBundlePath();
    initializeTestPluginDirectory();

    WKRetainPtr<WKStringRef> pageGroupIdentifier(AdoptWK, WKStringCreateWithUTF8CString("WebKitTestRunnerPageGroup"));
    m_pageGroup.adopt(WKPageGroupCreateWithIdentifier(pageGroupIdentifier.get()));

    m_context.adopt(WKContextCreateWithInjectedBundlePath(injectedBundlePath()));
    m_geolocationProvider = adoptPtr(new GeolocationProviderMock(m_context.get()));

    if (const char* dumpRenderTreeTemp = libraryPathForTesting()) {
        WKRetainPtr<WKStringRef> dumpRenderTreeTempWK(AdoptWK, WKStringCreateWithUTF8CString(dumpRenderTreeTemp));
        WKContextSetDatabaseDirectory(m_context.get(), dumpRenderTreeTempWK.get());
        WKContextSetLocalStorageDirectory(m_context.get(), dumpRenderTreeTempWK.get());
        WKContextSetDiskCacheDirectory(m_context.get(), dumpRenderTreeTempWK.get());
        WKContextSetCookieStorageDirectory(m_context.get(), dumpRenderTreeTempWK.get());

#if OS(WINDOWS)
        const char separator = '\\';
#else
        const char separator = '/';
#endif
        String iconDatabaseFileTemp = String::fromUTF8(dumpRenderTreeTemp) + separator + String(ASCIILiteral("WebpageIcons.db"));
        WKContextSetIconDatabasePath(m_context.get(), toWK(iconDatabaseFileTemp).get());
    }

    platformInitializeContext();

    WKContextInjectedBundleClient injectedBundleClient = {
        kWKContextInjectedBundleClientCurrentVersion,
        this,
        didReceiveMessageFromInjectedBundle,
        didReceiveSynchronousMessageFromInjectedBundle,
        0  
    };
    WKContextSetInjectedBundleClient(m_context.get(), &injectedBundleClient);

    WKNotificationManagerRef notificationManager = WKContextGetNotificationManager(m_context.get());
    WKNotificationProvider notificationKit = m_webNotificationProvider.provider();
    WKNotificationManagerSetProvider(notificationManager, &notificationKit);

    if (testPluginDirectory())
        WKContextSetAdditionalPluginsDirectory(m_context.get(), testPluginDirectory());

    createWebViewWithOptions(0);
}
