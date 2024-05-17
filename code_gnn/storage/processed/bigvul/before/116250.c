quint64 QQuickWebViewPrivate::exceededDatabaseQuota(const QString& databaseName, const QString& displayName, WKSecurityOriginRef securityOrigin, quint64 currentQuota, quint64 currentOriginUsage, quint64 currentDatabaseUsage, quint64 expectedUsage)
{
    Q_Q(QQuickWebView);
    QtDialogRunner dialogRunner(q);
    if (!dialogRunner.initForDatabaseQuotaDialog(databaseName, displayName, securityOrigin, currentQuota, currentOriginUsage, currentDatabaseUsage, expectedUsage))
        return 0;

    dialogRunner.run();

    return dialogRunner.wasAccepted() ? dialogRunner.databaseQuota() : 0;
}
