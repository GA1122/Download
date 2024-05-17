void GDataFileSystem::UpdateFileByEntryOnUIThread(
    const FileOperationCallback& callback,
    GDataEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!entry || !entry->AsGDataFile()) {
    base::MessageLoopProxy::current()->PostTask(
        FROM_HERE,
        base::Bind(callback,
                   GDATA_FILE_ERROR_NOT_FOUND));
    return;
  }
  GDataFile* file = entry->AsGDataFile();

  cache_->GetFileOnUIThread(
      file->resource_id(),
      file->file_md5(),
      base::Bind(&GDataFileSystem::OnGetFileCompleteForUpdateFile,
                 ui_weak_ptr_,
                 callback));
}
