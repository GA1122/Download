void Polkit1Backend::preAuthAction(const QString &action, QWidget *parent)
{
    if (!parent) {
        qCDebug(KAUTH) << "Parent widget does not exist, skipping";
        return;
    }

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(QLatin1String("org.kde.polkit-kde-authentication-agent-1"))) {
        if (qApp == 0 || !qobject_cast<QApplication *>(qApp)) {
            qCDebug(KAUTH) << "Not streaming parent as we are on a TTY application";
        }

        qulonglong wId = parent->effectiveWinId();

        QDBusMessage methodCall =
            QDBusMessage::createMethodCall(QLatin1String("org.kde.polkit-kde-authentication-agent-1"), QLatin1String("/org/kde/Polkit1AuthAgent"), QLatin1String("org.kde.Polkit1AuthAgent"),
                                           QLatin1String("setWIdForAction"));

        methodCall << action;
        methodCall << wId;

        QDBusPendingCall call = QDBusConnection::sessionBus().asyncCall(methodCall);
        call.waitForFinished();

        if (call.isError()) {
            qCWarning(KAUTH) << "ERROR while streaming the parent!!" << call.error();
        }
    } else {
        qCDebug(KAUTH) << "KDE polkit agent appears too old or not registered on the bus";
    }
}
