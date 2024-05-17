void DatabaseMessageFilter::OnDatabaseOpenFile(const string16& vfs_file_name,
                                               int desired_flags,
                                               IPC::Message* reply_msg) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  base::PlatformFile file_handle = base::kInvalidPlatformFileValue;
  string16 origin_identifier;
  string16 database_name;

  if (vfs_file_name.empty()) {
    VfsBackend::OpenTempFileInDirectory(db_tracker_->DatabaseDirectory(),
                                        desired_flags, &file_handle);
  } else if (DatabaseUtil::CrackVfsFileName(vfs_file_name, &origin_identifier,
                                            &database_name, NULL) &&
             !db_tracker_->IsDatabaseScheduledForDeletion(origin_identifier,
                                                          database_name)) {
      base::FilePath db_file =
          DatabaseUtil::GetFullFilePathForVfsFile(db_tracker_, vfs_file_name);
      if (!db_file.empty()) {
        if (db_tracker_->IsIncognitoProfile()) {
          db_tracker_->GetIncognitoFileHandle(vfs_file_name, &file_handle);
          if (file_handle == base::kInvalidPlatformFileValue) {
            VfsBackend::OpenFile(db_file,
                                 desired_flags | SQLITE_OPEN_DELETEONCLOSE,
                                 &file_handle);
            if (!(desired_flags & SQLITE_OPEN_DELETEONCLOSE))
              db_tracker_->SaveIncognitoFileHandle(vfs_file_name, file_handle);
          }
        } else {
          VfsBackend::OpenFile(db_file, desired_flags, &file_handle);
        }
      }
  }

  bool auto_close = !db_tracker_->HasSavedIncognitoFileHandle(vfs_file_name);
  IPC::PlatformFileForTransit target_handle =
      IPC::GetFileHandleForProcess(file_handle, peer_handle(), auto_close);

  DatabaseHostMsg_OpenFile::WriteReplyParams(reply_msg, target_handle);
  Send(reply_msg);
}
