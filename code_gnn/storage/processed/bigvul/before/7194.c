Action::AuthStatus PolicyKitBackend::actionStatus(const QString &action)
{
    PolkitQt::Auth::Result r = PolkitQt::Auth::isCallerAuthorized(action, QCoreApplication::applicationPid(),
                               false);
    switch (r) {
    case PolkitQt::Auth::Yes:
        return Action::StatusAuthorized;
    case PolkitQt::Auth::No:
    case PolkitQt::Auth::Unknown:
        return Action::StatusDenied;
    default:
        return Action::StatusAuthRequired;
    }
}
