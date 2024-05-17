AutomationProviderDownloadModelChangedObserver(
    AutomationProvider* provider,
    IPC::Message* reply_message,
    DownloadManager* download_manager)
    : provider_(provider->AsWeakPtr()),
      reply_message_(reply_message),
      ALLOW_THIS_IN_INITIALIZER_LIST(notifier_(download_manager, this)) {
}
