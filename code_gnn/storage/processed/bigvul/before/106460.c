WebPageCreationParameters WebPageProxy::creationParameters() const
{
    WebPageCreationParameters parameters;

    parameters.viewSize = m_pageClient->viewSize();
    parameters.isActive = m_pageClient->isViewWindowActive();
    parameters.isFocused = m_pageClient->isViewFocused();
    parameters.isVisible = m_pageClient->isViewVisible();
    parameters.isInWindow = m_pageClient->isViewInWindow();
    parameters.drawingAreaType = m_drawingArea->type();
    parameters.store = m_pageGroup->preferences()->store();
    parameters.pageGroupData = m_pageGroup->data();
    parameters.drawsBackground = m_drawsBackground;
    parameters.drawsTransparentBackground = m_drawsTransparentBackground;
    parameters.areMemoryCacheClientCallsEnabled = m_areMemoryCacheClientCallsEnabled;
    parameters.useFixedLayout = m_useFixedLayout;
    parameters.fixedLayoutSize = m_fixedLayoutSize;
    parameters.userAgent = userAgent();
    parameters.sessionState = SessionState(m_backForwardList->entries(), m_backForwardList->currentIndex());
    parameters.highestUsedBackForwardItemID = WebBackForwardListItem::highedUsedItemID();
    parameters.canRunBeforeUnloadConfirmPanel = m_uiClient.canRunBeforeUnloadConfirmPanel();
    parameters.canRunModal = m_uiClient.canRunModal();
    parameters.userSpaceScaleFactor = m_pageClient->userSpaceScaleFactor();

#if PLATFORM(MAC)
    parameters.isSmartInsertDeleteEnabled = m_isSmartInsertDeleteEnabled;
#endif

#if PLATFORM(WIN)
    parameters.nativeWindow = m_pageClient->nativeWindow();
#endif

    return parameters;
}
