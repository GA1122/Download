void PeopleHandler::PushSyncPrefs() {
#if !defined(OS_CHROMEOS)
  if (IsProfileAuthNeededOrHasErrors())
    return;
#endif

  ProfileSyncService* service = GetSyncService();
  if (!service || !service->IsEngineInitialized()) {
    return;
  }

  configuring_sync_ = true;
  DCHECK(service->IsEngineInitialized())
      << "Cannot configure sync until the sync engine is initialized";

  base::DictionaryValue args;

  const syncer::ModelTypeSet registered_types =
      service->GetRegisteredDataTypes();
  const syncer::ModelTypeSet preferred_types = service->GetPreferredDataTypes();
  const syncer::ModelTypeSet enforced_types = service->GetForcedDataTypes();
  syncer::ModelTypeNameMap type_names = syncer::GetUserSelectableTypeNameMap();
  for (syncer::ModelTypeNameMap::const_iterator it = type_names.begin();
       it != type_names.end(); ++it) {
    syncer::ModelType sync_type = it->first;
    const std::string key_name = it->second;
    args.SetBoolean(key_name + "Registered", registered_types.Has(sync_type));
    args.SetBoolean(key_name + "Synced", preferred_types.Has(sync_type));
    args.SetBoolean(key_name + "Enforced", enforced_types.Has(sync_type));
  }
  PrefService* pref_service = profile_->GetPrefs();
  syncer::SyncPrefs sync_prefs(pref_service);
  args.SetBoolean("syncAllDataTypes", sync_prefs.HasKeepEverythingSynced());
  args.SetBoolean("paymentsIntegrationEnabled",
                  autofill::prefs::IsPaymentsIntegrationEnabled(pref_service));
  args.SetBoolean("encryptAllData", service->IsEncryptEverythingEnabled());
  args.SetBoolean("encryptAllDataAllowed",
                  service->IsEncryptEverythingAllowed());

  args.SetBoolean("passphraseRequired", service->IsPassphraseRequired());

  args.SetBoolean("passphraseTypeIsCustom",
                  service->GetPassphraseType() ==
                      syncer::PassphraseType::CUSTOM_PASSPHRASE);
  base::Time passphrase_time = service->GetExplicitPassphraseTime();
  syncer::PassphraseType passphrase_type = service->GetPassphraseType();
  if (!passphrase_time.is_null()) {
    base::string16 passphrase_time_str =
        base::TimeFormatShortDate(passphrase_time);
    args.SetString("enterPassphraseBody",
                   GetStringFUTF16(IDS_SYNC_ENTER_PASSPHRASE_BODY_WITH_DATE,
                                   passphrase_time_str));
    args.SetString(
        "enterGooglePassphraseBody",
        GetStringFUTF16(IDS_SYNC_ENTER_GOOGLE_PASSPHRASE_BODY_WITH_DATE,
                        passphrase_time_str));
    switch (passphrase_type) {
      case syncer::PassphraseType::FROZEN_IMPLICIT_PASSPHRASE:
        args.SetString(
            "fullEncryptionBody",
            GetStringFUTF16(IDS_SYNC_FULL_ENCRYPTION_BODY_GOOGLE_WITH_DATE,
                            passphrase_time_str));
        break;
      case syncer::PassphraseType::CUSTOM_PASSPHRASE:
        args.SetString(
            "fullEncryptionBody",
            GetStringFUTF16(IDS_SYNC_FULL_ENCRYPTION_BODY_CUSTOM_WITH_DATE,
                            passphrase_time_str));
        break;
      default:
        args.SetString("fullEncryptionBody",
                       GetStringUTF16(IDS_SYNC_FULL_ENCRYPTION_BODY_CUSTOM));
        break;
    }
  } else if (passphrase_type == syncer::PassphraseType::CUSTOM_PASSPHRASE) {
    args.SetString("fullEncryptionBody",
                   GetStringUTF16(IDS_SYNC_FULL_ENCRYPTION_BODY_CUSTOM));
  }

  FireWebUIListener("sync-prefs-changed", args);
}
