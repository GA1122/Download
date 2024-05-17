void SyncBackendHost::ActivateDataType(
    DataTypeController* data_type_controller,
    ChangeProcessor* change_processor) {
  base::AutoLock lock(registrar_lock_);

  browser_sync::ModelSafeRoutingInfo::iterator i =
      registrar_.routing_info.find(data_type_controller->type());
  DCHECK(i != registrar_.routing_info.end());
  DCHECK((*i).second == GROUP_PASSIVE);
  syncable::ModelType type = data_type_controller->type();
  registrar_.routing_info[type] = data_type_controller->model_safe_group();

  DCHECK_EQ(processors_.count(type), 0U);
  processors_[type] = change_processor;

  change_processor->Start(profile_, GetUserShare());
}
