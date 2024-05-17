void SyncBackendHost::DeactivateDataType(
    DataTypeController* data_type_controller,
    ChangeProcessor* change_processor) {
  base::AutoLock lock(registrar_lock_);
  registrar_.routing_info.erase(data_type_controller->type());

  change_processor->Stop();
  std::map<syncable::ModelType, ChangeProcessor*>::size_type erased =
      processors_.erase(data_type_controller->type());
  DCHECK_EQ(erased, 1U);
}
