void WebPage::setDefaultLayoutSize(const Platform::IntSize& platformSize)
{
    WebCore::IntSize size = platformSize;
    if (size == d->m_defaultLayoutSize)
        return;

    d->setDefaultLayoutSize(size);
    bool needsLayout = d->setViewMode(d->viewMode());
    if (needsLayout) {
        d->setNeedsLayout();
        if (!d->isLoading())
            d->requestLayoutIfNeeded();
    }
}
