void GDataFileSystem::OnGetFileSizeCompleteForUpdateFile(
    const FileOperationCallback& callback,
    const std::string& resource_id,
    const std::string& md5,
    const FilePath& cache_file_path,
    GDataFileError* error,
    int64* file_size) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (*error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(*error);
    return;
  }

  directory_service_->GetEntryByResourceIdAsync(resource_id,
      base::Bind(&GDataFileSystem::OnGetFileCompleteForUpdateFileByEntry,
          ui_weak_ptr_,
          callback,
          md5,
          *file_size,
          cache_file_path));
}
