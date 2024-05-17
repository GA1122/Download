    ProfileSyncComponentsFactoryImpl::CreateTypedUrlSyncComponents(
        ProfileSyncService* profile_sync_service,
        history::HistoryBackend* history_backend,
        browser_sync::DataTypeErrorHandler* error_handler) {
  TypedUrlModelAssociator* model_associator =
      new TypedUrlModelAssociator(profile_sync_service,
                                  history_backend,
                                  error_handler);
  TypedUrlChangeProcessor* change_processor =
      new TypedUrlChangeProcessor(profile_,
                                  model_associator,
                                  history_backend,
                                  error_handler);
  return SyncComponents(model_associator, change_processor);
}
