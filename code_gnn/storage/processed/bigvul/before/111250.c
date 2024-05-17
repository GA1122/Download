void WebPage::notifyPageFullScreenExit()
{
    FOR_EACH_PLUGINVIEW(d->m_pluginViews)
        (*it)->handleFullScreenExitEvent();
}
