PassRefPtr<WebFrame> WebFrame::createSubframe(WebPage* page, const String& frameName, HTMLFrameOwnerElement* ownerElement)
{
    RefPtr<WebFrame> frame = create();

    page->send(Messages::WebPageProxy::DidCreateSubFrame(frame->frameID()));

    frame->init(page, frameName, ownerElement);

    return frame.release();
}
