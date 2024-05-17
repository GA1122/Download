void SyncBackendHost::ActivateDataType(
    syncable::ModelType type, ModelSafeGroup group,
    ChangeProcessor* change_processor) {
  registrar_->ActivateDataType(type, group, change_processor, GetUserShare());
}
