void QQuickWebViewFlickablePrivate::didChangeContentsSize(const QSize& newSize)
{
    Q_Q(QQuickWebView);

    pageView->setContentsSize(newSize);  
    m_viewportHandler->pageContentsSizeChanged(newSize, q->boundingRect().size().toSize());
}
