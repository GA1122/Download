void WebPagePrivate::notifyPageOnLoad()
{
    FOR_EACH_PLUGINVIEW(m_pluginViews)
        (*it)->handleOnLoadEvent();
}
