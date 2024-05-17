void WebPage::clearBackForwardList(bool keepCurrentPage) const
{
    BackForwardListImpl* backForwardList = static_cast<BackForwardListImpl*>(d->m_page->backForward()->client());
    RefPtr<HistoryItem> currentItem = backForwardList->currentItem();
    while (!backForwardList->entries().isEmpty())
        backForwardList->removeItem(backForwardList->entries().last().get());
    if (keepCurrentPage)
        backForwardList->addItem(currentItem);
}
