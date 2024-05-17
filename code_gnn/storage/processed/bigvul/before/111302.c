void WebPage::removeOverlay(WebOverlay* overlay)
{
#if USE(ACCELERATED_COMPOSITING)
    if (overlay->d->graphicsLayer()->parent() != d->overlayLayer())
        return;

    overlay->removeFromParent();
    overlay->d->clear();
    overlay->d->setPage(0);
#endif
}
