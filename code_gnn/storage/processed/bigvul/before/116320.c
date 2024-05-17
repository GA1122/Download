void QQuickWebViewPrivate::titleDidChange()
{
    Q_Q(QQuickWebView);

    emit q->titleChanged();
}
