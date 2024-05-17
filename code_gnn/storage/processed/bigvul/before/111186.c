void WebPage::inspectCurrentContextElement()
{
    if (isWebInspectorEnabled() && d->m_currentContextNode.get())
        d->m_page->inspectorController()->inspect(d->m_currentContextNode.get());
}
