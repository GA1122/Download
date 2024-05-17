void QQuickWebViewPrivate::handleProxyAuthenticationRequiredRequest(const QString& hostname, uint16_t port, const QString& prefilledUsername, QString& username, QString& password)
{
    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner(q);
    if (!dialogRunner.initForProxyAuthentication(hostname, port, prefilledUsername))
        return;

    dialogRunner.run();

    username = dialogRunner.username();
    password = dialogRunner.password();
}
