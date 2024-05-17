void SyncManager::SyncInternal::MaybeSetSyncTabsInNigoriNode(
    const syncable::ModelTypeSet enabled_types) {
  if (initialized_ && enabled_types.count(syncable::SESSIONS) > 0) {
    WriteTransaction trans(FROM_HERE, GetUserShare());
    WriteNode node(&trans);
    if (!node.InitByTagLookup(kNigoriTag)) {
      NOTREACHED() << "Unable to set 'sync_tabs' bit because Nigori node not "
                   << "found.";
      return;
    }

    sync_pb::NigoriSpecifics specifics(node.GetNigoriSpecifics());
    specifics.set_sync_tabs(true);
    node.SetNigoriSpecifics(specifics);
  }
}
