void QQuickWebViewExperimental::setCertificateVerificationDialog(QQmlComponent* certificateVerificationDialog)
{
    Q_D(QQuickWebView);
    if (d->certificateVerificationDialog == certificateVerificationDialog)
        return;
    d->certificateVerificationDialog = certificateVerificationDialog;
    emit certificateVerificationDialogChanged();
}
