void ProfileSyncService::OnMigrationNeededForTypes(
    syncable::ModelTypeSet types) {
  DCHECK(backend_initialized_);
  DCHECK(data_type_manager_.get());

  migrator_->MigrateTypes(types);
}
