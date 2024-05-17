QString QQuickWebViewPrivate::runJavaScriptPrompt(const QString& message, const QString& defaultValue, bool& ok)
{
    if (!promptDialog) {
        ok = true;
        return defaultValue;
    }

    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner;
    if (!dialogRunner.initForPrompt(promptDialog, q, message, defaultValue)) {
        ok = true;
        return defaultValue;
    }

    execDialogRunner(dialogRunner);

    ok = dialogRunner.wasAccepted();
    return dialogRunner.result();
}
