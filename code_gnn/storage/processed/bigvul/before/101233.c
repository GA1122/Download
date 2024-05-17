void SyncerProtoUtil::HandleMigrationDoneResponse(
  const sync_pb::ClientToServerResponse* response,
  sessions::SyncSession* session) {
  LOG_IF(ERROR, 0 >= response->migrated_data_type_id_size())
      << "MIGRATION_DONE but no types specified.";
  syncable::ModelTypeSet to_migrate;
  for (int i = 0; i < response->migrated_data_type_id_size(); i++) {
    to_migrate.insert(syncable::GetModelTypeFromExtensionFieldNumber(
        response->migrated_data_type_id(i)));
  }
  session->status_controller()->set_types_needing_local_migration(to_migrate);
}
