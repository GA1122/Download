void SyncBackendHost::Core::DeleteSyncDataFolder() {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  if (file_util::DirectoryExists(sync_data_folder_path_)) {
    if (!file_util::Delete(sync_data_folder_path_, true))
      SLOG(DFATAL) << "Could not delete the Sync Data folder.";
  }
}
