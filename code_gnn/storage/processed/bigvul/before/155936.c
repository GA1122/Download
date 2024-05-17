void PeopleHandler::HandleSetEncryption(const base::ListValue* args) {
  DCHECK(!sync_startup_tracker_);

  SyncConfigInfo configuration;
  const base::Value* callback_id = nullptr;
  ParseConfigurationArguments(args, &configuration, &callback_id);

  ProfileSyncService* service = GetSyncService();

  if (!service || !service->IsEngineInitialized()) {
    CloseSyncSetup();
    ResolveJavascriptCallback(*callback_id, base::Value(kDonePageStatus));
    return;
  }

  if (!service->IsEncryptEverythingAllowed())
    configuration.encrypt_all = false;

  if (configuration.encrypt_all)
    service->EnableEncryptEverything();

  bool passphrase_failed = false;
  if (!configuration.passphrase.empty()) {
    if (service->IsPassphraseRequired()) {
      passphrase_failed =
          !service->SetDecryptionPassphrase(configuration.passphrase);
    } else {
      if (configuration.set_new_passphrase &&
          !service->IsUsingSecondaryPassphrase()) {
        service->SetEncryptionPassphrase(configuration.passphrase);
      }
    }
  }

  if (passphrase_failed || service->IsPassphraseRequiredForDecryption()) {

    ResolveJavascriptCallback(*callback_id,
                              base::Value(kPassphraseFailedPageStatus));
  } else {
    ResolveJavascriptCallback(*callback_id, base::Value(kConfigurePageStatus));
  }

  if (configuration.encrypt_all)
    ProfileMetrics::LogProfileSyncInfo(ProfileMetrics::SYNC_ENCRYPT);
  if (!configuration.set_new_passphrase && !configuration.passphrase.empty())
    ProfileMetrics::LogProfileSyncInfo(ProfileMetrics::SYNC_PASSPHRASE);
}
