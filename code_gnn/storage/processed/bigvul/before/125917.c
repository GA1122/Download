AutomationProviderHistoryObserver::AutomationProviderHistoryObserver(
    AutomationProvider* provider,
    IPC::Message* reply_message)
    : provider_(provider->AsWeakPtr()),
      reply_message_(reply_message) {
}
