AppListSyncableService::~AppListSyncableService() {
  model_observer_.reset();
  model_pref_updater_.reset();

  STLDeleteContainerPairSecondPointers(sync_items_.begin(), sync_items_.end());
}
