void QQuickWebViewExperimental::setProxyAuthenticationDialog(QQmlComponent* proxyAuthenticationDialog)
{
    Q_D(QQuickWebView);
    if (d->proxyAuthenticationDialog == proxyAuthenticationDialog)
        return;
    d->proxyAuthenticationDialog = proxyAuthenticationDialog;
    emit proxyAuthenticationDialogChanged();
}
