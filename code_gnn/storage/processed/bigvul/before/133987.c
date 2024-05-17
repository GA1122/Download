void AppListSyncableService::RemoveUninstalledItem(const std::string& id) {
  RemoveSyncItem(id);
  model_->DeleteUninstalledItem(id);
  PruneEmptySyncFolders();
}
