void ProfileSyncService::OnSyncConfigureDone(
    DataTypeManager::ConfigureResult result) {
  if (failed_datatypes_handler_.UpdateFailedDatatypes(result.failed_data_types,
          FailedDatatypesHandler::STARTUP)) {
    ReconfigureDatatypeManager();
  }
}
