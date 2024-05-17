void ProfileSyncService::ActivateDataType(
    syncable::ModelType type, browser_sync::ModelSafeGroup group,
    ChangeProcessor* change_processor) {
  if (!backend_.get()) {
    NOTREACHED();
    return;
  }
  DCHECK(backend_initialized_);
  backend_->ActivateDataType(type, group, change_processor);
}
