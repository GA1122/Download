void QQuickWebViewPrivate::handleProxyAuthenticationRequiredRequest(const QString& hostname, uint16_t port, const QString& prefilledUsername, QString& username, QString& password)
{
    if (!proxyAuthenticationDialog)
        return;

    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner;
    if (!dialogRunner.initForProxyAuthentication(proxyAuthenticationDialog, q, hostname, port, prefilledUsername))
        return;

    execDialogRunner(dialogRunner);

    username = dialogRunner.username();
    password = dialogRunner.password();
}
