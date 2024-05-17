void SyncManager::SyncInternal::UpdateCryptographerAndNigoriCallback(
    const std::string& chrome_version,
    const base::Closure& done_callback,
    const std::string& session_name) {
  if (!directory()->initial_sync_ended_for_type(syncable::NIGORI)) {
    done_callback.Run();   
    return;
  }

  bool success = false;
  {
    WriteTransaction trans(FROM_HERE, GetUserShare());
    Cryptographer* cryptographer = trans.GetCryptographer();
    WriteNode node(&trans);

    if (node.InitByTagLookup(kNigoriTag) == sync_api::BaseNode::INIT_OK) {
      sync_pb::NigoriSpecifics nigori(node.GetNigoriSpecifics());
      Cryptographer::UpdateResult result = cryptographer->Update(nigori);
      if (result == Cryptographer::NEEDS_PASSPHRASE) {
        sync_pb::EncryptedData pending_keys;
        if (cryptographer->has_pending_keys())
          pending_keys = cryptographer->GetPendingKeys();
        FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                          OnPassphraseRequired(sync_api::REASON_DECRYPTION,
                                               pending_keys));
      }


      bool contains_this_device = false;
      for (int i = 0; i < nigori.device_information_size(); ++i) {
        const sync_pb::DeviceInformation& device_information =
            nigori.device_information(i);
        if (device_information.cache_guid() == directory()->cache_guid()) {
          if (device_information.chrome_version() != chrome_version) {
            sync_pb::DeviceInformation* mutable_device_information =
                nigori.mutable_device_information(i);
            mutable_device_information->set_chrome_version(
                chrome_version);
          }
          contains_this_device = true;
        }
      }

      if (!contains_this_device) {
        sync_pb::DeviceInformation* device_information =
            nigori.add_device_information();
        device_information->set_cache_guid(directory()->cache_guid());
#if defined(OS_CHROMEOS)
        device_information->set_platform("ChromeOS");
#elif defined(OS_LINUX)
        device_information->set_platform("Linux");
#elif defined(OS_MACOSX)
        device_information->set_platform("Mac");
#elif defined(OS_WIN)
        device_information->set_platform("Windows");
#endif
        device_information->set_name(session_name);
        device_information->set_chrome_version(chrome_version);
      }

      UpdateNigoriEncryptionState(cryptographer, &node);

      NotifyCryptographerState(cryptographer);
      allstatus_.SetEncryptedTypes(cryptographer->GetEncryptedTypes());

      success = cryptographer->is_ready();
    } else {
      NOTREACHED();
    }
  }

  if (success)
    RefreshEncryption();
  done_callback.Run();
}
