IntRect WebPagePrivate::transformedVisibleContentsRect() const
{
    return IntRect(transformedScrollPosition(), transformedViewportSize());
}
