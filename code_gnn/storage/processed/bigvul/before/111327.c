void WebPage::selectionCancelled()
{
    if (d->m_page->defersLoading()) {
        d->m_deferredTasks.append(adoptPtr(new DeferredTaskSelectionCancelled(d)));
        return;
    }
    DeferredTaskSelectionCancelled::finishOrCancel(d);
    d->m_selectionHandler->cancelSelection();
}
