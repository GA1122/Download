bool InitialSyncEndedForTypes(syncable::ModelTypeSet types,
                              sync_api::UserShare* share) {
  syncable::ScopedDirLookup lookup(share->dir_manager.get(),
                                   share->name);
  if (!lookup.good()) {
    DCHECK(false) << "ScopedDirLookup failed when checking initial sync";
    return false;
  }

  for (syncable::ModelTypeSet::const_iterator i = types.begin();
       i != types.end(); ++i) {
    if (!lookup->initial_sync_ended_for_type(*i))
      return false;
  }
  return true;
}
