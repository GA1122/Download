void WebPage::setJavaScriptCanAccessClipboard(bool enabled)
{
    d->m_page->settings()->setJavaScriptCanAccessClipboard(enabled);
}
