void WebPage::popupListClosed(int index)
{
    DeferredTaskPopupListSelectMultiple::finishOrCancel(d);
    if (d->m_page->defersLoading()) {
        d->m_deferredTasks.append(adoptPtr(new DeferredTaskPopupListSelectSingle(d, index)));
        return;
    }
    DeferredTaskPopupListSelectSingle::finishOrCancel(d);
    d->m_inputHandler->setPopupListIndex(index);
}
