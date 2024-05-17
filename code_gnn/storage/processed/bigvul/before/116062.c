void ExtensionSettingsHandler::HandleAutoUpdateMessage(const ListValue* args) {
  ExtensionUpdater* updater = extension_service_->updater();
  if (updater)
    updater->CheckNow();
}
