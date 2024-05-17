void WebPage::enableDNSPrefetch()
{
    d->m_page->settings()->setDNSPrefetchingEnabled(true);
}
