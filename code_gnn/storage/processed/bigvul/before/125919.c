AutomationProviderSearchEngineObserver::AutomationProviderSearchEngineObserver(
    AutomationProvider* provider,
    Profile* profile,
    IPC::Message* reply_message)
    : provider_(provider->AsWeakPtr()),
      profile_(profile),
      reply_message_(reply_message) {
}
