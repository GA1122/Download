void QQuickWebViewPrivate::handleAuthenticationRequiredRequest(const QString& hostname, const QString& realm, const QString& prefilledUsername, QString& username, QString& password)
{
    if (!authenticationDialog)
        return;

    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner;
    if (!dialogRunner.initForAuthentication(authenticationDialog, q, hostname, realm, prefilledUsername))
        return;

    execDialogRunner(dialogRunner);

    username = dialogRunner.username();
    password = dialogRunner.password();
}
