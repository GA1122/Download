VerifyResult SyncerUtil::VerifyUndelete(syncable::WriteTransaction* trans,
                                        const SyncEntity& update,
                                        syncable::MutableEntry* target) {
  CHECK(target->good());
  VLOG(1) << "Server update is attempting undelete. " << *target
          << "Update:" << SyncerProtoUtil::SyncEntityDebugString(update);
  if (target->Get(IS_DEL)) {
    DCHECK(target->Get(UNIQUE_CLIENT_TAG).empty())
        << "Doing move-aside undeletion on client-tagged item.";
    target->Put(ID, trans->directory()->NextId());
    target->Put(UNIQUE_CLIENT_TAG, "");
    target->Put(BASE_VERSION, CHANGES_VERSION);
    target->Put(SERVER_VERSION, 0);
    return VERIFY_SUCCESS;
  }
  if (update.version() < target->Get(SERVER_VERSION)) {
    LOG(WARNING) << "Update older than current server version for "
                 << *target << " Update:"
                 << SyncerProtoUtil::SyncEntityDebugString(update);
    return VERIFY_SUCCESS;   
  }
  return VERIFY_UNDECIDED;
}
