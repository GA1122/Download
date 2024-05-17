void WebPage::addCompositingThreadOverlay(WebOverlay* overlay)
{
#if USE(ACCELERATED_COMPOSITING)
    ASSERT(Platform::userInterfaceThreadMessageClient()->isCurrentThread());
    if (!d->compositor())
        return;

    overlay->d->setPage(d);
    d->compositor()->addOverlay(overlay->d->layerCompositingThread());
#endif
}
