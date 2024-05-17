DataTypeManager* ProfileSyncComponentsFactoryImpl::CreateDataTypeManager(
    SyncBackendHost* backend,
    const DataTypeController::TypeMap* controllers) {
  return new DataTypeManagerImpl(backend, controllers);
}
