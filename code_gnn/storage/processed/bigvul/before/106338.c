    SyncBackendHost::MakePendingConfigModeState(
        const DataTypeController::TypeMap& data_type_controllers,
        const syncable::ModelTypeSet& types,
        CancelableTask* ready_task,
        ModelSafeRoutingInfo* routing_info,
        sync_api::ConfigureReason reason) {
  PendingConfigureDataTypesState* state = new PendingConfigureDataTypesState();
  for (DataTypeController::TypeMap::const_iterator it =
           data_type_controllers.begin();
       it != data_type_controllers.end(); ++it) {
    syncable::ModelType type = it->first;
    if (types.count(type) == 0) {
      state->deleted_type = true;
      routing_info->erase(type);
    } else {
      if (routing_info->count(type) == 0) {
        (*routing_info)[type] = GROUP_PASSIVE;
        state->added_types.set(type);
      }
    }
  }

  state->ready_task.reset(ready_task);
  state->initial_types = types;
  state->reason = reason;
  return state;
}
