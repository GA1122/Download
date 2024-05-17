syncable::Id SyncerUtil::FindLocalIdToUpdate(
    syncable::BaseTransaction* trans,
    const SyncEntity& update) {

  const std::string& client_id = trans->directory()->cache_guid();

  if (update.has_client_defined_unique_tag() &&
      !update.client_defined_unique_tag().empty()) {


    syncable::Entry local_entry(trans, syncable::GET_BY_CLIENT_TAG,
                                update.client_defined_unique_tag());

    if (local_entry.good()) {
      if (local_entry.Get(ID).ServerKnows()) {
        if (local_entry.Get(ID) != update.id()) {
          LOG(WARNING) << "Duplicated client tag.";
          if (local_entry.Get(ID) < update.id()) {
            return syncable::GetNullId();
          }
        }
        return local_entry.Get(ID);
      } else {
        DCHECK(0 == local_entry.Get(BASE_VERSION) ||
               CHANGES_VERSION == local_entry.Get(BASE_VERSION));
        return local_entry.Get(ID);
      }
    }
  } else if (update.has_originator_cache_guid() &&
      update.originator_cache_guid() == client_id) {


    syncable::Id client_item_id = syncable::Id::CreateFromClientString(
        update.originator_client_item_id());
    DCHECK(!client_item_id.ServerKnows());
    syncable::Entry local_entry(trans, GET_BY_ID, client_item_id);

    if (local_entry.good() && !local_entry.Get(IS_DEL)) {
      int64 old_version = local_entry.Get(BASE_VERSION);
      int64 new_version = update.version();
      DCHECK_LE(old_version, 0);
      DCHECK_GT(new_version, 0);
      DCHECK(local_entry.Get(IS_UNSYNCED));

      DCHECK(!local_entry.Get(ID).ServerKnows());

      VLOG(1) << "Reuniting lost commit response IDs. server id: "
              << update.id() << " local id: " << local_entry.Get(ID)
              << " new version: " << new_version;

      return local_entry.Get(ID);
    }
  }
  return update.id();
}
