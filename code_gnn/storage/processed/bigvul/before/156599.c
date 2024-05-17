SessionStore::SessionStore(
    SyncSessionsClient* sessions_client,
    const SessionInfo& local_session_info,
    std::unique_ptr<ModelTypeStore> store,
    std::map<std::string, sync_pb::SessionSpecifics> initial_data,
    const syncer::EntityMetadataMap& initial_metadata,
    const RestoredForeignTabCallback& restored_foreign_tab_callback)
    : store_(std::move(store)),
      local_session_info_(local_session_info),
      session_tracker_(sessions_client),
      weak_ptr_factory_(this) {
  DCHECK(store_);

  DVLOG(1) << "Constructed session store with " << initial_data.size()
           << " restored entities and " << initial_metadata.size()
           << " metadata entries.";

  session_tracker_.InitLocalSession(local_session_info.session_tag,
                                    local_session_info.client_name,
                                    local_session_info.device_type);

  bool found_local_header = false;

  for (auto& storage_key_and_specifics : initial_data) {
    const std::string& storage_key = storage_key_and_specifics.first;
    SessionSpecifics& specifics = storage_key_and_specifics.second;

    if (!AreValidSpecifics(specifics)) {
      continue;
    }

    auto metadata_it = initial_metadata.find(storage_key);
    if (metadata_it == initial_metadata.end()) {
      continue;
    }

    const base::Time mtime =
        syncer::ProtoTimeToTime(metadata_it->second.modification_time());

    if (specifics.session_tag() != local_session_info.session_tag) {
      UpdateTrackerWithSpecifics(specifics, mtime, &session_tracker_);

      if (specifics.has_tab()) {
        restored_foreign_tab_callback.Run(specifics.tab(), mtime);
      }
    } else if (specifics.has_header()) {

      DCHECK(!found_local_header);
      found_local_header = true;

      UpdateTrackerWithSpecifics(specifics, mtime, &session_tracker_);
      DVLOG(1) << "Loaded local header.";
    } else {
      DCHECK(specifics.has_tab());

      DVLOG(1) << "Associating local tab " << specifics.tab().tab_id()
               << " with node " << specifics.tab_node_id();

      session_tracker_.ReassociateLocalTab(
          specifics.tab_node_id(),
          SessionID::FromSerializedValue(specifics.tab().tab_id()));
      UpdateTrackerWithSpecifics(specifics, mtime, &session_tracker_);
    }
  }

  for (const SyncedSession* session :
       session_tracker_.LookupAllForeignSessions(SyncedSessionTracker::RAW)) {
    session_tracker_.CleanupSession(session->session_tag);
  }
}
