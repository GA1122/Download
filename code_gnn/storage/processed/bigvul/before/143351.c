bool WebFrame::swap(WebFrame* frame)
{
    using std::swap;
    Frame* oldFrame = toImplBase()->frame();

    if (!oldFrame->prepareForCommit())
        return false;

    if (m_parent) {
        if (m_parent->m_firstChild == this)
            m_parent->m_firstChild = frame;
        if (m_parent->m_lastChild == this)
            m_parent->m_lastChild = frame;
        frame->m_parent = m_parent;
    }

    if (m_previousSibling) {
        m_previousSibling->m_nextSibling = frame;
        swap(m_previousSibling, frame->m_previousSibling);
    }
    if (m_nextSibling) {
        m_nextSibling->m_previousSibling = frame;
        swap(m_nextSibling, frame->m_nextSibling);
    }

    if (m_opener) {
        frame->setOpener(m_opener);
        setOpener(nullptr);
    }
    m_openedFrameTracker->transferTo(frame);

    FrameHost* host = oldFrame->host();
    AtomicString name = oldFrame->tree().name();
    AtomicString uniqueName = oldFrame->tree().uniqueName();
    FrameOwner* owner = oldFrame->owner();

    v8::HandleScope handleScope(v8::Isolate::GetCurrent());
    HashMap<DOMWrapperWorld*, v8::Local<v8::Object>> globals;
    oldFrame->getWindowProxyManager()->clearForNavigation();
    oldFrame->getWindowProxyManager()->releaseGlobals(globals);

    oldFrame->detach(FrameDetachType::Swap);

    if (frame->isWebLocalFrame()) {
        LocalFrame& localFrame = *toWebLocalFrameImpl(frame)->frame();
        DCHECK_EQ(owner, localFrame.owner());
        if (owner) {
            owner->setContentFrame(localFrame);
            if (owner->isLocal())
                toHTMLFrameOwnerElement(owner)->setWidget(localFrame.view());
        } else {
            localFrame.page()->setMainFrame(&localFrame);
        }
    } else {
        toWebRemoteFrameImpl(frame)->initializeCoreFrame(host, owner, name, uniqueName);
    }

    frame->toImplBase()->frame()->getWindowProxyManager()->setGlobals(globals);

    m_parent = nullptr;

    return true;
}
