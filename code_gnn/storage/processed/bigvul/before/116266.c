void QQuickWebViewPrivate::loadDidCommit()
{
    Q_Q(QQuickWebView);
    ASSERT(q->loading());

    emit q->navigationHistoryChanged();
    emit q->titleChanged();
}
