void WebPage::disableWebInspector()
{
    if (isWebInspectorEnabled()) {
        d->m_page->inspectorController()->disconnectFrontend();
        d->m_page->settings()->setDeveloperExtrasEnabled(false);
        d->setPreventsScreenDimming(false);
    }
}
