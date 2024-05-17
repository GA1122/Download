static inline WebPage::BackForwardId backForwardIdFromHistoryItem(HistoryItem* item)
{
    return reinterpret_cast<WebPage::BackForwardId>(item);
}
