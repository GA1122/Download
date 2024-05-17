std::string GetSessionTagWithPrefs(const std::string& cache_guid,
                                   SessionSyncPrefs* sync_prefs) {
  DCHECK(sync_prefs);
  const std::string persisted_guid = sync_prefs->GetSyncSessionsGUID();
  if (!persisted_guid.empty()) {
    DVLOG(1) << "Restoring persisted session sync guid: " << persisted_guid;
    return persisted_guid;
  }

  const std::string new_guid =
      base::StringPrintf("session_sync%s", cache_guid.c_str());
  DVLOG(1) << "Creating session sync guid: " << new_guid;
  sync_prefs->SetSyncSessionsGUID(new_guid);
  return new_guid;
}
