 void QQuickWebViewPrivate::enableMouseEvents()
 {
     Q_Q(QQuickWebView);
    q->setAcceptedMouseButtons(Qt::MouseButtonMask);
    q->setAcceptHoverEvents(true);
}
