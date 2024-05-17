void DatabaseMessageFilter::OnDatabaseDeleteFile(const string16& vfs_file_name,
                                                 const bool& sync_dir,
                                                 IPC::Message* reply_msg) {
  DatabaseDeleteFile(vfs_file_name, sync_dir, reply_msg, kNumDeleteRetries);
}
