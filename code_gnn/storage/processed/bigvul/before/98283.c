PassRefPtr<Frame> FrameLoaderClient::createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement,
                                                 const String& referrer, bool allowsScrolling, int marginWidth, int marginHeight)
{
    Frame* coreFrame = core(m_frame);

    ASSERT(core(getViewFromFrame(m_frame)) == coreFrame->page());

    RefPtr<Frame> childFrame = webkit_web_frame_init_with_web_view(getViewFromFrame(m_frame), ownerElement);

    coreFrame->tree()->appendChild(childFrame);

    childFrame->tree()->setName(name);
    childFrame->init();

    if (!childFrame->page())
        return 0;

    childFrame->loader()->loadURLIntoChildFrame(url, referrer, childFrame.get());

    if (!childFrame->tree()->parent())
        return 0;

    return childFrame.release();
}
