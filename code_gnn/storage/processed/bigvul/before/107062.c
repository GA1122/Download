void QQuickWebViewExperimental::setCertificateVerificationDialog(QDeclarativeComponent* certificateVerificationDialog)
{
    Q_D(QQuickWebView);
    if (d->certificateVerificationDialog == certificateVerificationDialog)
        return;
    d->certificateVerificationDialog = certificateVerificationDialog;
    emit certificateVerificationDialogChanged();
}
