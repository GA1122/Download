void WebPage::clearLocalStorage()
{
    if (PageGroup* group = d->m_page->groupPtr()) {
        if (StorageNamespace* storage = group->localStorage())
            storage->clearAllOriginsForDeletion();
    }
}
