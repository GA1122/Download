bool DBusHelperProxy::executeActions(const QList<QPair<QString, QVariantMap> > &list, const QString &helperID)
{
    QByteArray blob;
    QDataStream stream(&blob, QIODevice::WriteOnly);

    stream << list;

    QDBusConnection::systemBus().interface()->startService(helperID);

    if (!QDBusConnection::systemBus().connect(helperID, QLatin1String("/"), QLatin1String("org.kde.auth"), QLatin1String("remoteSignal"), this, SLOT(remoteSignalReceived(int,QString,QByteArray)))) {
        return false;
    }

    QDBusMessage message;
    message = QDBusMessage::createMethodCall(helperID, QLatin1String("/"), QLatin1String("org.kde.auth"), QLatin1String("performActions"));

    QList<QVariant> args;
    args << blob << BackendsManager::authBackend()->callerID();
    message.setArguments(args);

    QDBusPendingCall reply = QDBusConnection::systemBus().asyncCall(message);  
    if (reply.reply().type() == QDBusMessage::ErrorMessage) {
        return false;
    }

    return true;
}
