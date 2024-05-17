VerifyResult SyncerUtil::VerifyUpdateConsistency(
    syncable::WriteTransaction* trans,
    const SyncEntity& update,
    syncable::MutableEntry* target,
    const bool deleted,
    const bool is_directory,
    syncable::ModelType model_type) {

  CHECK(target->good());

  if (deleted)
    return VERIFY_SUCCESS;

  if (model_type == syncable::UNSPECIFIED) {
    return VERIFY_SKIP;
  }

  if (target->Get(SERVER_VERSION) > 0) {
    if (is_directory != target->Get(SERVER_IS_DIR) ||
        model_type != target->GetServerModelType()) {
      if (target->Get(IS_DEL)) {   
        return VERIFY_SKIP;
      } else {
        LOG(ERROR) << "Server update doesn't agree with previous updates. ";
        LOG(ERROR) << " Entry: " << *target;
        LOG(ERROR) << " Update: "
                   << SyncerProtoUtil::SyncEntityDebugString(update);
        return VERIFY_FAIL;
      }
    }

    if (!deleted && (target->Get(ID) == update.id()) &&
        (target->Get(SERVER_IS_DEL) ||
         (!target->Get(IS_UNSYNCED) && target->Get(IS_DEL) &&
          target->Get(BASE_VERSION) > 0))) {
      VerifyResult result =
          SyncerUtil::VerifyUndelete(trans, update, target);
      if (VERIFY_UNDECIDED != result)
        return result;
    }
  }
  if (target->Get(BASE_VERSION) > 0) {
    if (is_directory != target->Get(IS_DIR) ||
        model_type != target->GetModelType()) {
      LOG(ERROR) << "Server update doesn't agree with committed item. ";
      LOG(ERROR) << " Entry: " << *target;
      LOG(ERROR) << " Update: "
                 << SyncerProtoUtil::SyncEntityDebugString(update);
      return VERIFY_FAIL;
    }
    if (target->Get(ID) == update.id()) {
      if (target->Get(BASE_VERSION) == update.version() &&
          !target->Get(IS_UNSYNCED) &&
          !SyncerProtoUtil::Compare(*target, update)) {
        LOG(ERROR) << "Server update doesn't match local data with same "
            "version. A bug should be filed. Entry: " << *target <<
            "Update: " << SyncerProtoUtil::SyncEntityDebugString(update);
        return VERIFY_FAIL;
      }
      if (target->Get(SERVER_VERSION) > update.version()) {
        LOG(WARNING) << "We've already seen a more recent version.";
        LOG(WARNING) << " Entry: " << *target;
        LOG(WARNING) << " Update: "
                     << SyncerProtoUtil::SyncEntityDebugString(update);
        return VERIFY_SKIP;
      }
    }
  }
  return VERIFY_SUCCESS;
}
