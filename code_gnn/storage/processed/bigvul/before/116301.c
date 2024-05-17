void QQuickWebViewExperimental::setDatabaseQuotaDialog(QQmlComponent* databaseQuotaDialog)
{
    Q_D(QQuickWebView);
    if (d->databaseQuotaDialog == databaseQuotaDialog)
        return;
    d->databaseQuotaDialog = databaseQuotaDialog;
    emit databaseQuotaDialogChanged();
}
