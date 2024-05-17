void WebPage::notifyPageResume()
{
    FOR_EACH_PLUGINVIEW(d->m_pluginViews)
        (*it)->handleResumeEvent();
}
