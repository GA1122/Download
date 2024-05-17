    ProfileSyncComponentsFactoryImpl::CreateThemeSyncComponents(
        ProfileSyncService* profile_sync_service,
        DataTypeErrorHandler* error_handler) {
  ThemeModelAssociator* model_associator =
      new ThemeModelAssociator(profile_sync_service, error_handler);
  ThemeChangeProcessor* change_processor =
      new ThemeChangeProcessor(error_handler);
  return SyncComponents(model_associator, change_processor);
}
