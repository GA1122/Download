void WebPage::dispatchInspectorMessage(const BlackBerry::Platform::String& message)
{
    d->m_page->inspectorController()->dispatchMessageFromFrontend(message);
}
