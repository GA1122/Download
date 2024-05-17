void FrameLoader::setHistoryItemStateForCommit(HistoryCommitType historyCommitType, HistoryNavigationType navigationType)
{
    HistoryItem* oldItem = m_currentItem;
    if (historyCommitType == BackForwardCommit && m_provisionalItem)
        m_currentItem = m_provisionalItem.release();
    else
        m_currentItem = HistoryItem::create();
    m_currentItem->setURL(m_documentLoader->urlForHistory());
    m_currentItem->setDocumentState(m_frame->document()->formElementsState());
    m_currentItem->setTarget(m_frame->tree().uniqueName());
    m_currentItem->setReferrer(SecurityPolicy::generateReferrer(m_documentLoader->request().getReferrerPolicy(), m_currentItem->url(), m_documentLoader->request().httpReferrer()));
    m_currentItem->setFormInfoFromRequest(m_documentLoader->request());

    if (!oldItem || historyCommitType == BackForwardCommit)
        return;
    if (navigationType == HistoryNavigationType::DifferentDocument && (historyCommitType != HistoryInertCommit || !equalIgnoringFragmentIdentifier(oldItem->url(), m_currentItem->url())))
        return;
    m_currentItem->setDocumentSequenceNumber(oldItem->documentSequenceNumber());
    m_currentItem->setScrollPoint(oldItem->scrollPoint());
    m_currentItem->setVisualViewportScrollPoint(oldItem->visualViewportScrollPoint());
    m_currentItem->setPageScaleFactor(oldItem->pageScaleFactor());
    m_currentItem->setScrollRestorationType(oldItem->scrollRestorationType());

    if (historyCommitType == HistoryInertCommit && (navigationType == HistoryNavigationType::HistoryApi || oldItem->url() == m_currentItem->url())) {
        m_currentItem->setStateObject(oldItem->stateObject());
        m_currentItem->setItemSequenceNumber(oldItem->itemSequenceNumber());
    }
}
