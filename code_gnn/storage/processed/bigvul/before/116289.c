QString QQuickWebViewPrivate::runJavaScriptPrompt(const QString& message, const QString& defaultValue, bool& ok)
{
    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner(q);
    if (!dialogRunner.initForPrompt(message, defaultValue)) {
        ok = true;
        return defaultValue;
    }

    dialogRunner.run();

    ok = dialogRunner.wasAccepted();
    return dialogRunner.result();
}
