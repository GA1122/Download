    ProfileSyncComponentsFactoryImpl::CreatePasswordSyncComponents(
        ProfileSyncService* profile_sync_service,
        PasswordStore* password_store,
        DataTypeErrorHandler* error_handler) {
  PasswordModelAssociator* model_associator =
      new PasswordModelAssociator(profile_sync_service,
                                  password_store,
                                  error_handler);
  PasswordChangeProcessor* change_processor =
      new PasswordChangeProcessor(model_associator,
                                  password_store,
                                  error_handler);
  return SyncComponents(model_associator, change_processor);
}
