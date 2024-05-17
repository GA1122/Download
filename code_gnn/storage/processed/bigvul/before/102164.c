 void SyncManager::SyncInternal::OnServerConnectionEvent(
    const ServerConnectionEvent& event) {
  DCHECK(thread_checker_.CalledOnValidThread());
  allstatus_.HandleServerConnectionEvent(event);
  if (event.connection_code ==
      browser_sync::HttpResponse::SERVER_CONNECTION_OK) {
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                      OnAuthError(AuthError::None()));
  }

  if (event.connection_code == browser_sync::HttpResponse::SYNC_AUTH_ERROR) {
    observing_ip_address_changes_ = false;
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
        OnAuthError(AuthError(AuthError::INVALID_GAIA_CREDENTIALS)));
  }

  if (event.connection_code ==
      browser_sync::HttpResponse::SYNC_SERVER_ERROR) {
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
        OnAuthError(AuthError(AuthError::CONNECTION_FAILED)));
  }
}
