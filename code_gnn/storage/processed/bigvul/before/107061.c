void QQuickWebViewExperimental::setAuthenticationDialog(QDeclarativeComponent* authenticationDialog)
{
    Q_D(QQuickWebView);
    if (d->authenticationDialog == authenticationDialog)
        return;
    d->authenticationDialog = authenticationDialog;
    emit authenticationDialogChanged();
}
