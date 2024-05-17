void WebPage::notifyScreenPowerStateChanged(bool powered)
{
    FOR_EACH_PLUGINVIEW(d->m_pluginViews)
       (*it)->handleScreenPowerEvent(powered);
}
