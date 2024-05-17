void GDataFileSystem::OnGetEntryCompleteForCloseFile(
    const FilePath& file_path,
    const base::PlatformFileInfo& file_info,
    const FileOperationCallback& callback,
    GDataFileError error,
    GDataEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error);
    return;
  }

  DCHECK(entry);
  GDataFile* file = entry->AsGDataFile();
  if (!file || file->file_md5().empty() || file->is_hosted_document()) {
    if (!callback.is_null())
      callback.Run(GDATA_FILE_ERROR_INVALID_OPERATION);
    return;
  }
  DCHECK(!file->resource_id().empty());

  file->set_file_info(file_info);

  cache_->CommitDirtyOnUIThread(
      file->resource_id(),
      file->file_md5(),
      base::Bind(&GDataFileSystem::OnCommitDirtyInCacheCompleteForCloseFile,
                 ui_weak_ptr_,
                 callback));
}
