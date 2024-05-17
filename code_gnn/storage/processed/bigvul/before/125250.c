void DatabaseMessageFilter::OnDatabaseGetFileSize(
    const string16& vfs_file_name, IPC::Message* reply_msg) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  int64 size = 0;
  base::FilePath db_file =
      DatabaseUtil::GetFullFilePathForVfsFile(db_tracker_, vfs_file_name);
  if (!db_file.empty())
    size = VfsBackend::GetFileSize(db_file);

  DatabaseHostMsg_GetFileSize::WriteReplyParams(reply_msg, size);
  Send(reply_msg);
}
