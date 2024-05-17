void Document::dispose()
{
    ASSERT_WITH_SECURITY_IMPLICATION(!m_deletionHasBegun);
    m_docType = 0;
    m_focusedElement = 0;
    m_hoverNode = 0;
    m_activeElement = 0;
    m_titleElement = 0;
    m_documentElement = 0;
    m_contextFeatures = ContextFeatures::defaultSwitch();
    m_userActionElements.documentDidRemoveLastRef();
    m_associatedFormControls.clear();

    detachParser();

    m_registrationContext.clear();

    if (m_import) {
        m_import->wasDetachedFromDocument();
        m_import = 0;
    }

    destroyTreeScopeData();
    removeDetachedChildren();
    m_formController.clear();

    m_markers->detach();

    m_cssCanvasElements.clear();

    if (m_scriptedAnimationController)
        m_scriptedAnimationController->clearDocumentPointer();
    m_scriptedAnimationController.clear();

    if (svgExtensions())
        accessSVGExtensions()->pauseAnimations();

    m_lifecyle.advanceTo(DocumentLifecycle::Disposed);
    lifecycleNotifier()->notifyDocumentWasDisposed();
}
