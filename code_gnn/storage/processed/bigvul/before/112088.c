bool InitialSyncEndedForTypes(syncable::ModelTypeSet types,
                              sync_api::UserShare* share) {
  for (syncable::ModelTypeSet::Iterator i = types.First();
       i.Good(); i.Inc()) {
    if (!share->directory->initial_sync_ended_for_type(i.Get()))
      return false;
  }
  return true;
}
