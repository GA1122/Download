syncable::ModelTypeSet ProfileSyncService::GetRegisteredDataTypes() const {
  syncable::ModelTypeSet registered_types;
  for (DataTypeController::TypeMap::const_iterator it =
       data_type_controllers_.begin();
       it != data_type_controllers_.end(); ++it) {
    registered_types.Put(it->first);
  }
  return registered_types;
}
