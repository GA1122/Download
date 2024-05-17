void WebPage::notifyPagePause()
{
    FOR_EACH_PLUGINVIEW(d->m_pluginViews)
        (*it)->handlePauseEvent();
}
