void SyncManager::SyncInternal::UpdateEnabledTypes() {
  DCHECK(thread_checker_.CalledOnValidThread());
  ModelSafeRoutingInfo routes;
  registrar_->GetModelSafeRoutingInfo(&routes);
  syncable::ModelTypeSet enabled_types;
  for (ModelSafeRoutingInfo::const_iterator it = routes.begin();
       it != routes.end(); ++it) {
    enabled_types.insert(it->first);
  }
  sync_notifier_->UpdateEnabledTypes(enabled_types);
  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableSyncTabsForOtherClients)) {
    MaybeSetSyncTabsInNigoriNode(enabled_types);
  }
}
