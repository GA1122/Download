static inline HistoryItem* historyItemFromBackForwardId(WebPage::BackForwardId id)
{
    return reinterpret_cast<HistoryItem*>(id);
}
