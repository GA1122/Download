Platform::IntSize WebPage::viewportSize() const
{
    return d->transformedActualVisibleSize();
}
