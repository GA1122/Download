void ExtensionService::OnLoadedInstalledExtensions() {
  if (updater_.get()) {
    updater_->Start();
  }

  ready_ = true;
  NotificationService::current()->Notify(
      NotificationType::EXTENSIONS_READY,
      Source<Profile>(profile_),
      NotificationService::NoDetails());
}
