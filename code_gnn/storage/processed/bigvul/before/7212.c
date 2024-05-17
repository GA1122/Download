void Polkit1Backend::updateCachedActions(const PolkitQt1::ActionDescription::List &actions)
{
    m_knownActions.clear();
    Q_FOREACH (const PolkitQt1::ActionDescription &action, actions) {
        m_knownActions << action.actionId();
    }
    m_flyingActions = false;
}
