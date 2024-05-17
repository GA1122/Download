void WebPage::notifyDeviceIdleStateChange(bool enterIdle)
{
    FOR_EACH_PLUGINVIEW(d->m_pluginViews)
        (*it)->handleIdleEvent(enterIdle);
}
