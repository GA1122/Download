void WebPageProxy::processDidCrash()
{
    ASSERT(m_pageClient);

    m_isValid = false;

    if (m_mainFrame)
        m_urlAtProcessExit = m_mainFrame->url();

    m_mainFrame = 0;

    m_drawingArea = nullptr;

#if ENABLE(INSPECTOR)
    if (m_inspector) {
        m_inspector->invalidate();
        m_inspector = 0;
    }
#endif

#if ENABLE(FULLSCREEN_API)
    if (m_fullScreenManager) {
        m_fullScreenManager->invalidate();
        m_fullScreenManager = 0;
    }
#endif

    if (m_openPanelResultListener) {
        m_openPanelResultListener->invalidate();
        m_openPanelResultListener = 0;
    }

    m_geolocationPermissionRequestManager.invalidateRequests();

    m_toolTip = String();

    m_mainFrameHasHorizontalScrollbar = false;
    m_mainFrameHasVerticalScrollbar = false;

    m_mainFrameIsPinnedToLeftSide = false;
    m_mainFrameIsPinnedToRightSide = false;

    invalidateCallbackMap(m_voidCallbacks);
    invalidateCallbackMap(m_dataCallbacks);
    invalidateCallbackMap(m_stringCallbacks);
    m_loadDependentStringCallbackIDs.clear();
    invalidateCallbackMap(m_scriptValueCallbacks);
    invalidateCallbackMap(m_computedPagesCallbacks);
    invalidateCallbackMap(m_validateCommandCallbacks);

    Vector<WebEditCommandProxy*> editCommandVector;
    copyToVector(m_editCommandSet, editCommandVector);
    m_editCommandSet.clear();
    for (size_t i = 0, size = editCommandVector.size(); i < size; ++i)
        editCommandVector[i]->invalidate();
    m_pageClient->clearAllEditCommands();

    m_activePopupMenu = 0;

    m_estimatedProgress = 0.0;

    m_pendingLearnOrIgnoreWordMessageCount = 0;

    m_pageClient->processDidCrash();
    m_loaderClient.processDidCrash(this);

    m_keyEventQueue.clear();
}
