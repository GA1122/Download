bool DirectoryBackingStore::SaveChanges(
    const Directory::SaveChangesSnapshot& snapshot) {
  sqlite3* dbhandle = LazyGetSaveHandle();

  bool save_info =
    (Directory::KERNEL_SHARE_INFO_DIRTY == snapshot.kernel_info_status);
  if (snapshot.dirty_metas.size() < 1 && !save_info)
    return true;

  sqlite_utils::SQLTransaction transaction(dbhandle);
  if (SQLITE_OK != transaction.BeginExclusive())
    return false;

  for (EntryKernelSet::const_iterator i = snapshot.dirty_metas.begin();
       i != snapshot.dirty_metas.end(); ++i) {
    DCHECK(i->is_dirty());
    if (!SaveEntryToDB(*i))
      return false;
  }

  if (!DeleteEntries(snapshot.metahandles_to_purge))
    return false;

  if (save_info) {
    const Directory::PersistedKernelInfo& info = snapshot.kernel_info;
    sqlite_utils::SQLStatement update;
    update.prepare(dbhandle, "UPDATE share_info "
                   "SET store_birthday = ?, "
                   "next_id = ?, "
                   "notification_state = ? ");

    update.bind_string(0, info.store_birthday);
    update.bind_int64(1, info.next_id);
    update.bind_blob(2, info.notification_state.data(),
                     info.notification_state.size());

    if (!(SQLITE_DONE == update.step() &&
          SQLITE_OK == update.reset() &&
          1 == update.changes())) {
      return false;
    }

    for (int i = FIRST_REAL_MODEL_TYPE; i < MODEL_TYPE_COUNT; ++i) {
      sqlite_utils::SQLStatement op;
      op.prepare(dbhandle, "INSERT OR REPLACE INTO models (model_id, "
      "progress_marker, initial_sync_ended) VALUES ( ?, ?, ?)");
      string model_id = ModelTypeEnumToModelId(ModelTypeFromInt(i));
      string progress_marker;
      info.download_progress[i].SerializeToString(&progress_marker);
      op.bind_blob(0, model_id.data(), model_id.length());
      op.bind_blob(1, progress_marker.data(), progress_marker.length());
      op.bind_bool(2, info.initial_sync_ended[i]);

      if (!(SQLITE_DONE == op.step() &&
            SQLITE_OK == op.reset() &&
            1 == op.changes())) {
        return false;
      }
    }
  }

  return (SQLITE_OK == transaction.Commit());
}
