void WebPage::getBackForwardList(SharedArray<BackForwardEntry>& result) const
{
    HistoryItemVector entries = static_cast<BackForwardListImpl*>(d->m_page->backForward()->client())->entries();
    result.reset(new BackForwardEntry[entries.size()], entries.size());

    for (unsigned i = 0; i < entries.size(); ++i) {
        RefPtr<HistoryItem> entry = entries[i];
        BackForwardEntry& resultEntry = result[i];
        resultEntry.url = entry->urlString();
        resultEntry.originalUrl = entry->originalURLString();
        resultEntry.title = entry->title();
        resultEntry.networkToken = entry->viewState().networkToken;
        resultEntry.lastVisitWasHTTPNonGet = entry->lastVisitWasHTTPNonGet();
        resultEntry.id = backForwardIdFromHistoryItem(entry.get());

        entry->ref();
    }
}
