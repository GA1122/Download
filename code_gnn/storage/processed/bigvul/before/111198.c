bool WebPage::isWebInspectorEnabled()
{
    return d->m_page->settings()->developerExtrasEnabled();
}
