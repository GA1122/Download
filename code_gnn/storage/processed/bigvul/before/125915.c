AutomationProviderDownloadUpdatedObserver(
    AutomationProvider* provider,
    IPC::Message* reply_message,
    bool wait_for_open,
    bool incognito)
    : provider_(provider->AsWeakPtr()),
      reply_message_(reply_message),
      wait_for_open_(wait_for_open),
      incognito_(incognito) {
}
