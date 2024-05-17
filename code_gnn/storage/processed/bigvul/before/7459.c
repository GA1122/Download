bool DBusHelperProxy::isCallerAuthorized(const QString &action, const QByteArray &callerID)
{
    switch (BackendsManager::authBackend()->extraCallerIDVerificationMethod()) {
        case AuthBackend::NoExtraCallerIDVerificationMethod:
        break;

        case AuthBackend::VerifyAgainstDBusServiceName:
            if (message().service().toUtf8() != callerID) {
                return false;
            }
        break;

        case AuthBackend::VerifyAgainstDBusServicePid:
            if (connection().interface()->servicePid(message().service()).value() != callerID.toUInt()) {
                return false;
            }
        break;
    }

    return BackendsManager::authBackend()->isCallerAuthorized(action, callerID);
}
