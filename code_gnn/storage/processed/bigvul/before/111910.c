void ProfileSyncService::OnDisableDatatype(
    syncable::ModelType type,
    const tracked_objects::Location& from_here,
    std::string message) {
  DeactivateDataType(type);

  SyncError error(from_here, message, type);

  std::list<SyncError> errors;
  errors.push_back(error);

  failed_datatypes_handler_.UpdateFailedDatatypes(errors,
      FailedDatatypesHandler::RUNTIME);

  MessageLoop::current()->PostTask(FROM_HERE,
      base::Bind(&ProfileSyncService::ReconfigureDatatypeManager,
                 weak_factory_.GetWeakPtr()));
}
