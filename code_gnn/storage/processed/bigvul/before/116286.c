void QQuickWebViewPrivate::runJavaScriptAlert(const QString& alertText)
{
    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner(q);
    if (!dialogRunner.initForAlert(alertText))
        return;

    dialogRunner.run();
}
