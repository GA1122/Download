WebPageProxy::WebPageProxy(PageClient* pageClient, WebContext* context, WebPageGroup* pageGroup, uint64_t pageID)
    : m_pageClient(pageClient)
    , m_context(context)
    , m_pageGroup(pageGroup)
    , m_mainFrame(0)
    , m_userAgent(standardUserAgent())
    , m_geolocationPermissionRequestManager(this)
    , m_estimatedProgress(0)
    , m_isInWindow(m_pageClient->isViewInWindow())
    , m_isVisible(m_pageClient->isViewVisible())
    , m_backForwardList(WebBackForwardList::create(this))
    , m_textZoomFactor(1)
    , m_pageZoomFactor(1)
    , m_viewScaleFactor(1)
    , m_drawsBackground(true)
    , m_drawsTransparentBackground(false)
    , m_areMemoryCacheClientCallsEnabled(true)
    , m_useFixedLayout(false)
    , m_isValid(true)
    , m_isClosed(false)
    , m_isInPrintingMode(false)
    , m_isPerformingDOMPrintOperation(false)
    , m_inDecidePolicyForMIMEType(false)
    , m_syncMimeTypePolicyActionIsValid(false)
    , m_syncMimeTypePolicyAction(PolicyUse)
    , m_syncMimeTypePolicyDownloadID(0)
    , m_inDecidePolicyForNavigationAction(false)
    , m_syncNavigationActionPolicyActionIsValid(false)
    , m_syncNavigationActionPolicyAction(PolicyUse)
    , m_syncNavigationActionPolicyDownloadID(0)
    , m_processingWheelEvent(false)
    , m_processingMouseMoveEvent(false)
    , m_pageID(pageID)
#if PLATFORM(MAC)
    , m_isSmartInsertDeleteEnabled(TextChecker::isSmartInsertDeleteEnabled())
#endif
    , m_spellDocumentTag(0)
    , m_hasSpellDocumentTag(false)
    , m_pendingLearnOrIgnoreWordMessageCount(0)
    , m_mainFrameHasCustomRepresentation(false)
    , m_currentDragOperation(DragOperationNone)
    , m_mainFrameHasHorizontalScrollbar(false)
    , m_mainFrameHasVerticalScrollbar(false)
    , m_mainFrameIsPinnedToLeftSide(false)
    , m_mainFrameIsPinnedToRightSide(false)
{
#ifndef NDEBUG
    webPageProxyCounter.increment();
#endif

    WebContext::statistics().wkPageCount++;

    m_pageGroup->addPage(this);
}
