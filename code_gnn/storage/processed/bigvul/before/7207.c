 bool Polkit1Backend::isCallerAuthorized(const QString &action, QByteArray callerID)
 {
     PolkitQt1::SystemBusNameSubject subject(QString::fromUtf8(callerID));
    authority->checkAuthorization(action, subject, PolkitQt1::Authority::AllowUserInteraction);
    e.exec();

    switch (e.result()) {
    case PolkitQt1::Authority::Yes:
        return true;
    default:
        return false;
    }

    return false;
}
