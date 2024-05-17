Action::AuthStatus Polkit1Backend::actionStatus(const QString &action)
{
    PolkitQt1::SystemBusNameSubject subject(QString::fromUtf8(callerID()));
    PolkitQt1::Authority::Result r = PolkitQt1::Authority::instance()->checkAuthorizationSync(action, subject,
                                                                                              PolkitQt1::Authority::None);
    switch (r) {
    case PolkitQt1::Authority::Yes:
        return Action::Authorized;
    case PolkitQt1::Authority::No:
    case PolkitQt1::Authority::Unknown:
        return Action::Denied;
    default:
        return Action::AuthRequired;
    }
}
