bool QQuickWebViewPrivate::runJavaScriptConfirm(const QString& message)
{
    if (!confirmDialog)
        return true;

    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner;
    if (!dialogRunner.initForConfirm(confirmDialog, q, message))
        return true;

    execDialogRunner(dialogRunner);

    return dialogRunner.wasAccepted();
}
