void PolicyKitBackend::setupAction(const QString &action)
{
    connect(PolkitQt::Context::instance(), SIGNAL(configChanged()),
            this, SLOT(checkForResultChanged()));
    connect(PolkitQt::Context::instance(), SIGNAL(consoleKitDBChanged()),
            this, SLOT(checkForResultChanged()));

    m_cachedResults[action] = actionStatus(action);
}
