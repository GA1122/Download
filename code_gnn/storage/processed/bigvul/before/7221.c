Action::AuthStatus PolicyKitBackend::authorizeAction(const QString &action)
{
    switch (PolkitQt::Auth::computeAndObtainAuth(action)) {
    case PolkitQt::Auth::Yes:
        return Action::Authorized;
    default:
        return Action::Denied;
    }
}
