void SyncManager::SyncInternal::OnSyncEngineEvent(
    const SyncEngineEvent& event) {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (!HaveObservers()) {
    LOG(INFO)
        << "OnSyncEngineEvent returning because observers_.size() is zero";
    return;
  }

  if (event.what_happened == SyncEngineEvent::SYNC_CYCLE_ENDED) {
    ModelSafeRoutingInfo enabled_types;
    registrar_->GetModelSafeRoutingInfo(&enabled_types);
    {
      sync_api::ReadTransaction trans(FROM_HERE, GetUserShare());
      Cryptographer* cryptographer = trans.GetCryptographer();
      if (cryptographer->has_pending_keys()) {
        VLOG(1) << "OnPassPhraseRequired Sent";
        ObserverList<SyncManager::Observer> temp_obs_list;
        CopyObservers(&temp_obs_list);
        FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                          OnPassphraseRequired(sync_api::REASON_DECRYPTION));
      } else if (!cryptographer->is_ready() &&
                 event.snapshot->initial_sync_ended.test(syncable::NIGORI)) {
        VLOG(1) << "OnPassphraseRequired sent because cryptographer is not "
                << "ready";
        ObserverList<SyncManager::Observer> temp_obs_list;
        CopyObservers(&temp_obs_list);
        FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                          OnPassphraseRequired(sync_api::REASON_ENCRYPTION));
      }

      allstatus_.SetCryptographerReady(cryptographer->is_ready());
      allstatus_.SetCryptoHasPendingKeys(cryptographer->has_pending_keys());
      allstatus_.SetEncryptedTypes(cryptographer->GetEncryptedTypes());

    }

    if (!initialized_) {
      LOG(INFO) << "OnSyncCycleCompleted not sent because sync api is not "
                << "initialized";
      return;
    }

    if (!event.snapshot->has_more_to_sync) {
      VLOG(1) << "OnSyncCycleCompleted sent";
      ObserverList<SyncManager::Observer> temp_obs_list;
      CopyObservers(&temp_obs_list);
      FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                        OnSyncCycleCompleted(event.snapshot));
    }

    bool is_notifiable_commit =
        (event.snapshot->syncer_status.num_successful_commits > 0);
    if (is_notifiable_commit) {
      allstatus_.IncrementNotifiableCommits();
      if (sync_notifier_.get()) {
        const syncable::ModelTypeSet& changed_types =
            syncable::ModelTypePayloadMapToSet(event.snapshot->source.types);
        sync_notifier_->SendNotification(changed_types);
      } else {
        VLOG(1) << "Not sending notification: sync_notifier_ is NULL";
      }
    }
  }

  if (event.what_happened == SyncEngineEvent::STOP_SYNCING_PERMANENTLY) {
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                      OnStopSyncingPermanently());
    return;
  }

  if (event.what_happened == SyncEngineEvent::CLEAR_SERVER_DATA_SUCCEEDED) {
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                      OnClearServerDataSucceeded());
    return;
  }

  if (event.what_happened == SyncEngineEvent::CLEAR_SERVER_DATA_FAILED) {
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                      OnClearServerDataFailed());
    return;
  }

  if (event.what_happened == SyncEngineEvent::UPDATED_TOKEN) {
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                      OnUpdatedToken(event.updated_token));
    return;
  }

  if (event.what_happened == SyncEngineEvent::ACTIONABLE_ERROR) {
    ObserverList<SyncManager::Observer> temp_obs_list;
    CopyObservers(&temp_obs_list);
    FOR_EACH_OBSERVER(SyncManager::Observer, temp_obs_list,
                      OnActionableError(
                          event.snapshot->errors.sync_protocol_error));
    return;
  }

}
