void DatabaseMessageFilter::DatabaseDeleteFile(const string16& vfs_file_name,
                                               bool sync_dir,
                                               IPC::Message* reply_msg,
                                               int reschedule_count) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));

  int error_code = SQLITE_IOERR_DELETE;
  base::FilePath db_file =
      DatabaseUtil::GetFullFilePathForVfsFile(db_tracker_, vfs_file_name);
  if (!db_file.empty()) {
    if (db_tracker_->IsIncognitoProfile()) {
      const string16 wal_suffix(ASCIIToUTF16("-wal"));
      string16 sqlite_suffix;

      if (!db_tracker_->HasSavedIncognitoFileHandle(vfs_file_name) &&
          DatabaseUtil::CrackVfsFileName(vfs_file_name,
                                         NULL, NULL, &sqlite_suffix) &&
          sqlite_suffix == wal_suffix) {
        error_code = SQLITE_OK;
      } else if (db_tracker_->CloseIncognitoFileHandle(vfs_file_name)) {
        error_code = SQLITE_OK;
      }
    } else {
      error_code = VfsBackend::DeleteFile(db_file, sync_dir);
    }

    if ((error_code == SQLITE_IOERR_DELETE) && reschedule_count) {
      BrowserThread::PostDelayedTask(
          BrowserThread::FILE, FROM_HERE,
          base::Bind(&DatabaseMessageFilter::DatabaseDeleteFile, this,
                     vfs_file_name, sync_dir, reply_msg, reschedule_count - 1),
          base::TimeDelta::FromMilliseconds(kDelayDeleteRetryMs));
      return;
    }
  }

  DatabaseHostMsg_DeleteFile::WriteReplyParams(reply_msg, error_code);
  Send(reply_msg);
}
