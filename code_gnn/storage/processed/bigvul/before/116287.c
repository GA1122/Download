bool QQuickWebViewPrivate::runJavaScriptConfirm(const QString& message)
{
    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner(q);
    if (!dialogRunner.initForConfirm(message))
        return true;

    dialogRunner.run();

    return dialogRunner.wasAccepted();
}
