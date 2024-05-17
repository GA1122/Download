void WebPage::popupListClosed(int size, const bool* selecteds)
{
    DeferredTaskPopupListSelectSingle::finishOrCancel(d);
    if (d->m_page->defersLoading()) {
        d->m_deferredTasks.append(adoptPtr(new DeferredTaskPopupListSelectMultiple(d, size, selecteds)));
        return;
    }
    DeferredTaskPopupListSelectMultiple::finishOrCancel(d);
    d->m_inputHandler->setPopupListIndexes(size, selecteds);
}
