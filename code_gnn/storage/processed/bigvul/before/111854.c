    ProfileSyncComponentsFactoryImpl::CreateGenericChangeProcessor(
        ProfileSyncService* profile_sync_service,
        browser_sync::DataTypeErrorHandler* error_handler,
        const base::WeakPtr<SyncableService>& local_service) {
  sync_api::UserShare* user_share = profile_sync_service->GetUserShare();
  return new GenericChangeProcessor(error_handler,
                                    local_service,
                                    user_share);
}
