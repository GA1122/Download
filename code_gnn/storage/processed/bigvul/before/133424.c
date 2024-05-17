bool SVGImage::dataChanged(bool allDataReceived)
{
    TRACE_EVENT0("webkit", "SVGImage::dataChanged");

    if (!data()->size())
        return true;

    if (allDataReceived) {
        static FrameLoaderClient* dummyFrameLoaderClient = new EmptyFrameLoaderClient;

        Page::PageClients pageClients;
        fillWithEmptyClients(pageClients);
        m_chromeClient = adoptPtr(new SVGImageChromeClient(this));
        pageClients.chromeClient = m_chromeClient.get();

        m_page = adoptPtr(new Page(pageClients));
        m_page->settings().setScriptEnabled(false);
        m_page->settings().setPluginsEnabled(false);
        m_page->settings().setAcceleratedCompositingEnabled(false);

        RefPtr<LocalFrame> frame = LocalFrame::create(FrameInit::create(&m_page->frameHost(), dummyFrameLoaderClient));
        frame->setView(FrameView::create(frame.get()));
        frame->init();
        FrameLoader& loader = frame->loader();
        loader.forceSandboxFlags(SandboxAll);

        frame->view()->setScrollbarsSuppressed(true);
        frame->view()->setCanHaveScrollbars(false);  
        frame->view()->setTransparent(true);  

        loader.load(FrameLoadRequest(0, blankURL(), SubstituteData(data(), "image/svg+xml", "UTF-8", KURL(), ForceSynchronousLoad)));
        m_intrinsicSize = containerSize();
    }

    return m_page;
}
