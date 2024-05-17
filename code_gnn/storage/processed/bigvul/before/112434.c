void Document::detach()
{
    ASSERT(attached());
    ASSERT(!m_inPageCache);

#if ENABLE(POINTER_LOCK)
    if (page())
        page()->pointerLockController()->documentDetached(this);
#endif

    if (this == topDocument())
        clearAXObjectCache();

    stopActiveDOMObjects();
    m_eventQueue->close();
#if ENABLE(FULLSCREEN_API)
    m_fullScreenChangeEventTargetQueue.clear();
    m_fullScreenErrorEventTargetQueue.clear();
#endif

#if ENABLE(REQUEST_ANIMATION_FRAME)
    if (m_scriptedAnimationController)
        m_scriptedAnimationController->clearDocumentPointer();
    m_scriptedAnimationController.clear();
#endif

    RenderObject* render = renderer();

    documentWillBecomeInactive();

#if ENABLE(SHARED_WORKERS)
    SharedWorkerRepository::documentDetached(this);
#endif

    if (m_frame) {
        FrameView* view = m_frame->view();
        if (view)
            view->detachCustomScrollbars();

    }

    setRenderer(0);
    
#if ENABLE(FULLSCREEN_API)
    if (m_fullScreenRenderer)
        setFullScreenRenderer(0);
#endif

    m_hoverNode = 0;
    m_focusedNode = 0;
    m_activeElement = 0;

    ContainerNode::detach();

    unscheduleStyleRecalc();

    if (render)
        render->destroy();

#if ENABLE(TOUCH_EVENTS)
    if (m_touchEventTargets && m_touchEventTargets->size() && parentDocument())
        parentDocument()->didRemoveEventTargetNode(this);
#endif

    m_frame = 0;
    m_renderArena.clear();

    if (m_mediaQueryMatcher)
        m_mediaQueryMatcher->documentDestroyed();
}
