void QQuickWebViewFlickablePrivate::updateContentsSize(const QSizeF& size)
{
    ASSERT(flickProvider);

    if (!userDidOverrideContentWidth)
        flickProvider->setContentWidth(size.width());
    if (!userDidOverrideContentHeight)
        flickProvider->setContentHeight(size.height());
}
