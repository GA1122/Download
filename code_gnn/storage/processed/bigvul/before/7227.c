void Polkit1Backend::checkForResultChanged()
{
    foreach(const QString &action, m_cachedResults.keys()) {
        if (m_cachedResults[action] != actionStatus(action)) {
            m_cachedResults[action] = actionStatus(action);
            emit actionStatusChanged(action, m_cachedResults[action]);
        }
    }

    PolkitQt1::Authority::instance()->enumerateActions();
    m_flyingActions = true;
}
