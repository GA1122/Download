void WebPage::notifyPageBackground()
{
    FOR_EACH_PLUGINVIEW(d->m_pluginViews)
        (*it)->handleBackgroundEvent();
}
