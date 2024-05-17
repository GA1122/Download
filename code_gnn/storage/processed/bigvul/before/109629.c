void Document::detach(const AttachContext& context)
{
    m_lifecyle.advanceTo(DocumentLifecycle::Stopping);

    ASSERT(confusingAndOftenMisusedAttached());

    if (page())
        page()->documentDetached(this);

    if (this == topDocument())
        clearAXObjectCache();

    stopActiveDOMObjects();
    m_eventQueue->close();

    if (m_scriptedAnimationController)
        m_scriptedAnimationController->clearDocumentPointer();
    m_scriptedAnimationController.clear();

    if (svgExtensions())
        accessSVGExtensions()->pauseAnimations();

    RenderObject* render = renderer();

    documentWillBecomeInactive();

    SharedWorkerRepository::documentDetached(this);

    if (m_frame) {
        FrameView* view = m_frame->view();
        if (view)
            view->detachCustomScrollbars();
    }

    setRenderer(0);

    m_hoverNode = 0;
    m_focusedElement = 0;
    m_activeElement = 0;

    m_styleResolverThrowawayTimer.stop();

    ContainerNode::detach(context);

    unscheduleStyleRecalc();

    clearStyleResolver();

    if (render)
        render->destroy();

    if (m_touchEventTargets && m_touchEventTargets->size() && parentDocument())
        parentDocument()->didRemoveEventTargetNode(this);

    m_frame = 0;

    if (m_mediaQueryMatcher)
        m_mediaQueryMatcher->documentDestroyed();

    lifecycleNotifier()->notifyDocumentWasDetached();
    m_lifecyle.advanceTo(DocumentLifecycle::Stopped);
}
