  void Create(const syncer::DeviceInfo& device_info,
              SessionStore::FactoryCompletionCallback callback) {
    const std::string cache_guid = device_info.guid();
    DCHECK(!cache_guid.empty());

    SessionStore::SessionInfo session_info;
    session_info.client_name = device_info.client_name();
    session_info.device_type = device_info.device_type();
    session_info.session_tag = GetSessionTagWithPrefs(
        cache_guid, sessions_client_->GetSessionSyncPrefs());

    DVLOG(1) << "Initiating creation of session store";

    sessions_client_->GetStoreFactory().Run(
        syncer::SESSIONS,
        base::BindOnce(&FactoryImpl::OnStoreCreated, base::AsWeakPtr(this),
                       session_info, std::move(callback)));
  }
