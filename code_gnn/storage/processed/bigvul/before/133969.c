AppListModel* AppListSyncableService::GetModel() {
  if (!apps_builder_)
    BuildModel();

  return model_.get();
}
