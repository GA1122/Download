Action::AuthStatus PolicyKitBackend::authorizeAction(const QString &action)
{
    switch (PolkitQt::Auth::computeAndObtainAuth(action)) {
    case PolkitQt::Auth::Yes:
        return Action::StatusAuthorized;
    default:
        return Action::StatusDenied;
    }
}
