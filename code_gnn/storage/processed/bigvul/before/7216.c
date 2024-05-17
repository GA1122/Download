Action::AuthStatus DBusHelperProxy::authorizeAction(const QString& action, const QString& helperID)
{
    if (!m_actionsInProgress.isEmpty()) {
        return Action::Error;
    }

    QDBusConnection::systemBus().interface()->startService(helperID);

    QDBusMessage message;
    message = QDBusMessage::createMethodCall(helperID, QLatin1String("/"), QLatin1String("org.kde.auth"), QLatin1String("authorizeAction"));

    QList<QVariant> args;
    args << action << BackendsManager::authBackend()->callerID();
    message.setArguments(args);

    m_actionsInProgress.push_back(action);

    QEventLoop e;
    QDBusPendingCall pendingCall = QDBusConnection::systemBus().asyncCall(message);
    QDBusPendingCallWatcher watcher(pendingCall, this);
    connect(&watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), &e, SLOT(quit()));
    e.exec();

    m_actionsInProgress.removeOne(action);

    QDBusMessage reply = pendingCall.reply();

    if (reply.type() == QDBusMessage::ErrorMessage || reply.arguments().size() != 1) {
        return Action::Error;
    }

    return static_cast<Action::AuthStatus>(reply.arguments().first().toUInt());
}
