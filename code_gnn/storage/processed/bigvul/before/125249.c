void DatabaseMessageFilter::OnDatabaseGetFileAttributes(
    const string16& vfs_file_name,
    IPC::Message* reply_msg) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  int32 attributes = -1;
  base::FilePath db_file =
      DatabaseUtil::GetFullFilePathForVfsFile(db_tracker_, vfs_file_name);
  if (!db_file.empty())
    attributes = VfsBackend::GetFileAttributes(db_file);

  DatabaseHostMsg_GetFileAttributes::WriteReplyParams(
      reply_msg, attributes);
  Send(reply_msg);
}
