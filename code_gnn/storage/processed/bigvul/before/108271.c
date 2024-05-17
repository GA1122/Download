void FrameLoader::navigateWithinDocument(HistoryItem* item)
{
    ASSERT(item->documentSequenceNumber() == history()->currentItem()->documentSequenceNumber());

    history()->saveScrollPositionAndViewStateToItem(history()->currentItem());
    if (FrameView* view = m_frame->view())
        view->setWasScrolledByUser(false);

    history()->setCurrentItem(item);
        
    loadInSameDocument(item->url(), item->stateObject(), false);

    history()->restoreScrollPositionAndViewState();
}
