SavePackageNotificationObserver::SavePackageNotificationObserver(
    DownloadManager* download_manager,
    AutomationProvider* automation,
    IPC::Message* reply_message)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message) {
  content::Source<DownloadManager> source(download_manager);
  registrar_.Add(this, content::NOTIFICATION_SAVE_PACKAGE_SUCCESSFULLY_FINISHED,
                 source);
}
