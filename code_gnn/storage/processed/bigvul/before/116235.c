QPointF QQuickWebViewPrivate::contentPos() const
{
    Q_Q(const QQuickWebView);
    return QPointF(q->contentX(), q->contentY());
}
