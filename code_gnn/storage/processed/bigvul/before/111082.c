void WebPage::clearCache()
{
    clearMemoryCaches();
    clearAppCache(d->m_page->groupName());
}
