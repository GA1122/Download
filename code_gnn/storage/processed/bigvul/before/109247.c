 Page* InspectorOverlay::overlayPage()
 {
     if (m_overlayPage)
        return m_overlayPage.get();

    static FrameLoaderClient* dummyFrameLoaderClient =  new EmptyFrameLoaderClient;
    Page::PageClients pageClients;
    fillWithEmptyClients(pageClients);
    ASSERT(!m_overlayChromeClient);
    m_overlayChromeClient = adoptPtr(new InspectorOverlayChromeClient(m_page->chrome().client(), this));
    pageClients.chromeClient = m_overlayChromeClient.get();
    m_overlayPage = adoptPtr(new Page(pageClients));
    m_overlayPage->setGroupType(Page::InspectorPageGroup);

    Settings& settings = m_page->settings();
    Settings& overlaySettings = m_overlayPage->settings();

    overlaySettings.setStandardFontFamily(settings.standardFontFamily());
    overlaySettings.setSerifFontFamily(settings.serifFontFamily());
    overlaySettings.setSansSerifFontFamily(settings.sansSerifFontFamily());
    overlaySettings.setCursiveFontFamily(settings.cursiveFontFamily());
    overlaySettings.setFantasyFontFamily(settings.fantasyFontFamily());
    overlaySettings.setPictographFontFamily(settings.pictographFontFamily());
    overlaySettings.setMinimumFontSize(settings.minimumFontSize());
    overlaySettings.setMinimumLogicalFontSize(settings.minimumLogicalFontSize());
    overlaySettings.setMediaEnabled(false);
    overlaySettings.setScriptEnabled(true);
    overlaySettings.setPluginsEnabled(false);
    overlaySettings.setLoadsImagesAutomatically(true);

    RefPtr<Frame> frame = Frame::create(m_overlayPage.get(), 0, dummyFrameLoaderClient);
    frame->setView(FrameView::create(frame.get()));
    frame->init();
    FrameLoader& loader = frame->loader();
    frame->view()->setCanHaveScrollbars(false);
    frame->view()->setTransparent(true);
    ASSERT(loader.activeDocumentLoader());
    DocumentWriter* writer = loader.activeDocumentLoader()->beginWriting("text/html", "UTF-8");
    writer->addData(reinterpret_cast<const char*>(InspectorOverlayPage_html), sizeof(InspectorOverlayPage_html));
    loader.activeDocumentLoader()->endWriting(writer);
    v8::Isolate* isolate = toIsolate(frame.get());
    v8::HandleScope handleScope(isolate);
    v8::Handle<v8::Context> frameContext = frame->script().currentWorldContext();
    v8::Context::Scope contextScope(frameContext);
    v8::Handle<v8::Value> overlayHostObj = toV8(m_overlayHost.get(), v8::Handle<v8::Object>(), isolate);
    v8::Handle<v8::Object> global = frameContext->Global();
    global->Set(v8::String::New("InspectorOverlayHost"), overlayHostObj);

#if OS(WIN)
    evaluateInOverlay("setPlatform", "windows");
#elif OS(MACOSX)
    evaluateInOverlay("setPlatform", "mac");
#elif OS(POSIX)
    evaluateInOverlay("setPlatform", "linux");
#endif

    return m_overlayPage.get();
}
