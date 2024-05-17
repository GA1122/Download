syncer::ModelTypeSet SupervisedUserService::GetPreferredDataTypes() const {
  if (!ProfileIsSupervised())
    return syncer::ModelTypeSet();

  syncer::ModelTypeSet result;
  result.Put(syncer::EXTENSIONS);
  result.Put(syncer::EXTENSION_SETTINGS);
  result.Put(syncer::APPS);
  result.Put(syncer::APP_SETTINGS);
  result.Put(syncer::APP_NOTIFICATIONS);
  result.Put(syncer::APP_LIST);
  return result;
}
