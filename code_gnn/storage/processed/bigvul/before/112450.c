void Document::dispose()
{
    ASSERT(!m_deletionHasBegun);
    m_docType = 0;
    m_focusedNode = 0;
    m_hoverNode = 0;
    m_activeElement = 0;
    m_titleElement = 0;
    m_documentElement = 0;
    m_contextFeatures = ContextFeatures::defaultSwitch();
    m_userActionElements.documentDidRemoveLastRef();
#if ENABLE(FULLSCREEN_API)
    m_fullScreenElement = 0;
    m_fullScreenElementStack.clear();
#endif

    detachParser();

#if ENABLE(CUSTOM_ELEMENTS)
    m_registry.clear();
#endif

    destroyTreeScopeData();
    removeDetachedChildren();

    m_markers->detach();

    m_cssCanvasElements.clear();

#if ENABLE(REQUEST_ANIMATION_FRAME)
    if (m_scriptedAnimationController)
        m_scriptedAnimationController->clearDocumentPointer();
    m_scriptedAnimationController.clear();
#endif
}
