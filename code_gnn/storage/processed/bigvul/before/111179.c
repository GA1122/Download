void WebPagePrivate::init(const BlackBerry::Platform::String& pageGroupName)
{
    ChromeClientBlackBerry* chromeClient = new ChromeClientBlackBerry(this);
    ContextMenuClientBlackBerry* contextMenuClient = 0;
#if ENABLE(CONTEXT_MENUS)
    contextMenuClient = new ContextMenuClientBlackBerry();
#endif
    EditorClientBlackBerry* editorClient = new EditorClientBlackBerry(this);
    DragClientBlackBerry* dragClient = 0;
#if ENABLE(DRAG_SUPPORT)
    dragClient = new DragClientBlackBerry();
#endif
#if ENABLE(INSPECTOR)
    m_inspectorClient = new InspectorClientBlackBerry(this);
#endif

    FrameLoaderClientBlackBerry* frameLoaderClient = new FrameLoaderClientBlackBerry();

    Page::PageClients pageClients;
    pageClients.chromeClient = chromeClient;
    pageClients.contextMenuClient = contextMenuClient;
    pageClients.editorClient = editorClient;
    pageClients.dragClient = dragClient;
    pageClients.inspectorClient = m_inspectorClient;

    m_page = new Page(pageClients);
#if !defined(PUBLIC_BUILD) || !PUBLIC_BUILD
    if (isRunningDrt()) {
        GeolocationClientMock* mock = new GeolocationClientMock();
        WebCore::provideGeolocationTo(m_page, mock);
        mock->setController(WebCore::GeolocationController::from(m_page));
    } else
#endif
        WebCore::provideGeolocationTo(m_page, new GeolocationClientBlackBerry(this));
#if !defined(PUBLIC_BUILD) || !PUBLIC_BUILD
    if (getenv("drtRun"))
        WebCore::provideDeviceOrientationTo(m_page, new DeviceOrientationClientMock);
    else
#endif
        WebCore::provideDeviceOrientationTo(m_page, new DeviceOrientationClientBlackBerry(this));

    WebCore::provideDeviceMotionTo(m_page, new DeviceMotionClientBlackBerry(this));
#if ENABLE(VIBRATION)
    WebCore::provideVibrationTo(m_page, new VibrationClientBlackBerry());
#endif

#if ENABLE(BATTERY_STATUS)
    WebCore::provideBatteryTo(m_page, new WebCore::BatteryClientBlackBerry(this));
#endif

#if ENABLE(MEDIA_STREAM)
    WebCore::provideUserMediaTo(m_page, new UserMediaClientImpl(m_webPage));
#endif

#if ENABLE(NOTIFICATIONS) || ENABLE(LEGACY_NOTIFICATIONS)
    WebCore::provideNotification(m_page, new NotificationClientBlackBerry(this));
#endif

#if ENABLE(NAVIGATOR_CONTENT_UTILS)
    WebCore::provideNavigatorContentUtilsTo(m_page, new NavigatorContentUtilsClientBlackBerry(this));
#endif

#if ENABLE(NETWORK_INFO)
    WebCore::provideNetworkInfoTo(m_page, new WebCore::NetworkInfoClientBlackBerry(this));
#endif

    m_webSettings = WebSettings::createFromStandardSettings();
    m_webSettings->setUserAgentString(defaultUserAgent());
    m_page->setDeviceScaleFactor(m_webSettings->devicePixelRatio());

    m_page->addLayoutMilestones(DidFirstVisuallyNonEmptyLayout);

#if USE(ACCELERATED_COMPOSITING)
    m_tapHighlight = DefaultTapHighlight::create(this);
    m_selectionOverlay = SelectionOverlay::create(this);
    m_page->settings()->setAcceleratedCompositingForFixedPositionEnabled(true);
#endif

    m_webSettings->setPageGroupName(pageGroupName);
    m_webSettings->setDelegate(this);
    didChangeSettings(m_webSettings);

    RefPtr<Frame> newFrame = Frame::create(m_page,   0, frameLoaderClient);

    m_mainFrame = newFrame.get();
    frameLoaderClient->setFrame(m_mainFrame, this);
    m_mainFrame->init();

    m_inRegionScroller = adoptPtr(new InRegionScroller(this));

#if ENABLE(WEBGL)
    m_page->settings()->setWebGLEnabled(true);
#endif
#if ENABLE(ACCELERATED_2D_CANVAS)
    m_page->settings()->setCanvasUsesAcceleratedDrawing(true);
    m_page->settings()->setAccelerated2dCanvasEnabled(true);
#endif

    m_page->settings()->setInteractiveFormValidationEnabled(true);
    m_page->settings()->setAllowUniversalAccessFromFileURLs(false);
    m_page->settings()->setAllowFileAccessFromFileURLs(false);
    m_page->settings()->setFixedPositionCreatesStackingContext(true);

    m_backingStoreClient = BackingStoreClient::create(m_mainFrame,   0, m_webPage);
    m_backingStore = m_backingStoreClient->backingStore();

    m_webkitThreadViewportAccessor = new WebKitThreadViewportAccessor(this);

    blockClickRadius = int(roundf(0.35 * Platform::Graphics::Screen::primaryScreen()->pixelsPerInch(0).width()));  

    m_page->settings()->setDelegateSelectionPaint(true);

#if ENABLE(REQUEST_ANIMATION_FRAME)
    m_page->windowScreenDidChange((PlatformDisplayID)0);
#endif

#if ENABLE(WEB_TIMING)
    m_page->settings()->setMemoryInfoEnabled(true);
#endif

#if USE(ACCELERATED_COMPOSITING)
    Platform::userInterfaceThreadMessageClient()->dispatchSyncMessage(
            createMethodCallMessage(&WebPagePrivate::createCompositor, this));
#endif
}
