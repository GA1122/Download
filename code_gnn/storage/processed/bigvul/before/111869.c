void ProfileSyncService::DeactivateDataType(syncable::ModelType type) {
  if (!backend_.get())
    return;
  backend_->DeactivateDataType(type);
}
