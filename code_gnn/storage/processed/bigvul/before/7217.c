ActionReply DBusHelperProxy::executeAction(const QString &action, const QString &helperID, const QVariantMap &arguments)
{
    if (!m_actionsInProgress.isEmpty()) {
        return ActionReply::HelperBusyReply;
    }

    QByteArray blob;
    QDataStream stream(&blob, QIODevice::WriteOnly);

    stream << arguments;

    QDBusConnection::systemBus().interface()->startService(helperID);

    if (!QDBusConnection::systemBus().connect(helperID, QLatin1String("/"), QLatin1String("org.kde.auth"), QLatin1String("remoteSignal"), this, SLOT(remoteSignalReceived(int,QString,QByteArray)))) {
        ActionReply errorReply = ActionReply::DBusErrorReply;
        errorReply.setErrorDescription(i18n("DBus Backend error: connection to helper failed. %1",
                                            QDBusConnection::systemBus().lastError().message()));
        return errorReply;
    }

    QDBusMessage message;
    message = QDBusMessage::createMethodCall(helperID, QLatin1String("/"), QLatin1String("org.kde.auth"), QLatin1String("performAction"));

    QList<QVariant> args;
    args << action << BackendsManager::authBackend()->callerID() << blob;
    message.setArguments(args);

    m_actionsInProgress.push_back(action);

    QEventLoop e;
    QDBusPendingCall pendingCall = QDBusConnection::systemBus().asyncCall(message);
    QDBusPendingCallWatcher watcher(pendingCall, this);
    connect(&watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), &e, SLOT(quit()));
    e.exec();

    QDBusMessage reply = pendingCall.reply();

    if (reply.type() == QDBusMessage::ErrorMessage) {
        ActionReply r = ActionReply::DBusErrorReply;
        r.setErrorDescription(i18n("DBus Backend error: could not contact the helper. "
                                   "Connection error: %1. Message error: %2", QDBusConnection::systemBus().lastError().message(),
                                   reply.errorMessage()));
        qDebug() << reply.errorMessage();

        m_actionsInProgress.removeOne(action);

        return r;
    }

    if (reply.arguments().size() != 1) {
        ActionReply errorReply = ActionReply::DBusErrorReply;
        errorReply.setErrorDescription(i18n("DBus Backend error: received corrupt data from helper %1 %2",
                                            reply.arguments().size(), QDBusConnection::systemBus().lastError().message()));

        m_actionsInProgress.removeOne(action);

        return errorReply;
    }

    return ActionReply::deserialize(reply.arguments().first().toByteArray());
}
