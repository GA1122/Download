void PolicyKitBackend::checkForResultChanged()
{
    QHash<QString, Action::AuthStatus>::iterator i;
    for (i = m_cachedResults.begin(); i != m_cachedResults.end(); ++i) {
        if (i.value() != actionStatus(i.key())) {
            i.value() = actionStatus(i.key());
            emit actionStatusChanged(i.key(), i.value());
        }
    }
}
