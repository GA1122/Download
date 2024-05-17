void WebPage::touchEventCancel()
{
    d->m_pluginMayOpenNewTab = false;
    if (d->m_page->defersLoading())
        return;
}
