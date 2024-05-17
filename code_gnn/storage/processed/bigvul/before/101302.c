VerifyResult SyncerUtil::VerifyNewEntry(
    const SyncEntity& update,
    syncable::Entry* target,
    const bool deleted) {
  if (target->good()) {
    return VERIFY_UNDECIDED;
  }
  if (deleted) {
    return VERIFY_SKIP;
  }

  return VERIFY_SUCCESS;
}
