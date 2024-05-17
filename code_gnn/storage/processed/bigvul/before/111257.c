void WebPage::notifySwipeEvent()
{
    if (d->m_fullScreenPluginView.get())
       d->m_fullScreenPluginView->handleSwipeEvent();
    else
       notifyFullScreenVideoExited(true);
}
