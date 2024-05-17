void FrameLoader::loadItem(HistoryItem* item, FrameLoadType loadType)
{
    HistoryItem* currentItem = history()->currentItem();
    bool sameDocumentNavigation = currentItem && item != currentItem
        && item->documentSequenceNumber() == currentItem->documentSequenceNumber();

#if ENABLE(WML)
    if (frameContainsWMLContent(m_frame))
        sameDocumentNavigation = false;
#endif

    if (sameDocumentNavigation)
        navigateWithinDocument(item);
    else
        navigateToDifferentDocument(item, loadType);
}
