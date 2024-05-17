DataTypeManager* ProfileSyncComponentsFactoryImpl::CreateDataTypeManager(
    const syncer::WeakHandle<syncer::DataTypeDebugInfoListener>&
        debug_info_listener,
    SyncBackendHost* backend,
    const DataTypeController::TypeMap* controllers,
    DataTypeManagerObserver* observer,
    const FailedDatatypesHandler* failed_datatypes_handler) {
  return new DataTypeManagerImpl(debug_info_listener,
                                 backend,
                                 controllers,
                                 observer,
                                 failed_datatypes_handler);
}
