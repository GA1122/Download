    ProfileSyncComponentsFactoryImpl::CreateSessionSyncComponents(
       ProfileSyncService* profile_sync_service,
        DataTypeErrorHandler* error_handler) {
  SessionModelAssociator* model_associator =
      new SessionModelAssociator(profile_sync_service, error_handler);
  SessionChangeProcessor* change_processor =
      new SessionChangeProcessor(error_handler, model_associator);
  return SyncComponents(model_associator, change_processor);
}
