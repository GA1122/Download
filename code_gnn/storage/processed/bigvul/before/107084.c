void QQuickWebViewFlickablePrivate::updateViewportSize()
{
    Q_Q(QQuickWebView);
    QSize viewportSize = q->boundingRect().size().toSize();

    if (viewportSize.isEmpty() || !interactionEngine)
        return;

    flickProvider->setViewportSize(viewportSize);

    webPageProxy->setViewportSize(viewportSize);

    interactionEngine->applyConstraints(computeViewportConstraints());
    _q_commitScaleChange();
}
