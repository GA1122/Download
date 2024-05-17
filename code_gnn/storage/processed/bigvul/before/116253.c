void QQuickWebViewPrivate::handleAuthenticationRequiredRequest(const QString& hostname, const QString& realm, const QString& prefilledUsername, QString& username, QString& password)
{
    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner(q);
    if (!dialogRunner.initForAuthentication(hostname, realm, prefilledUsername))
        return;

    dialogRunner.run();

    username = dialogRunner.username();
    password = dialogRunner.password();
}
