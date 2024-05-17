bool WebPage::isDNSPrefetchEnabled() const
{
    return d->m_page->settings()->dnsPrefetchingEnabled();
}
