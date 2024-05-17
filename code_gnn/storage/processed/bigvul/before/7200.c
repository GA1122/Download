Polkit1Backend::Polkit1Backend()
    : AuthBackend()
    , m_flyingActions(false)
{
    setCapabilities(AuthorizeFromHelperCapability | CheckActionExistenceCapability | PreAuthActionCapability);

    connect(PolkitQt1::Authority::instance(), SIGNAL(configChanged()),
            this, SLOT(checkForResultChanged()));
    connect(PolkitQt1::Authority::instance(), SIGNAL(consoleKitDBChanged()),
            this, SLOT(checkForResultChanged()));
    connect(PolkitQt1::Authority::instance(), SIGNAL(enumerateActionsFinished(PolkitQt1::ActionDescription::List)),
            this, SLOT(updateCachedActions(PolkitQt1::ActionDescription::List)));

    m_flyingActions = true;
    PolkitQt1::Authority::instance()->enumerateActions();
}
