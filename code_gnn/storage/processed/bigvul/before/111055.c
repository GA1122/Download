void WebPage::addOverlay(WebOverlay* overlay)
{
#if USE(ACCELERATED_COMPOSITING)
    if (overlay->d->graphicsLayer()) {
        overlay->d->setPage(d);
        d->overlayLayer()->addChild(overlay->d->graphicsLayer());
    }
#endif
}
