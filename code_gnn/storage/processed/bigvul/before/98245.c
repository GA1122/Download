PassRefPtr<WebFrame> WebFrame::createMainFrame(WebPage* page)
{
    RefPtr<WebFrame> frame = create();

    page->send(Messages::WebPageProxy::DidCreateMainFrame(frame->frameID()));

    frame->init(page, String(), 0);

    return frame.release();
}
