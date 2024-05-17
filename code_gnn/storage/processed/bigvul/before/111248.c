void WebPage::notifyPageForeground()
{
    FOR_EACH_PLUGINVIEW(d->m_pluginViews)
        (*it)->handleForegroundEvent();
}
