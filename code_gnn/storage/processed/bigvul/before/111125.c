void WebPage::disableDNSPrefetch()
{
    d->m_page->settings()->setDNSPrefetchingEnabled(false);
}
