void WebPage::notifyPageFullScreenAllowed()
{
    FOR_EACH_PLUGINVIEW(d->m_pluginViews)
        (*it)->handleFullScreenAllowedEvent();
}
