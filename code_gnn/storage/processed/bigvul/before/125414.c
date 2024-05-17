void GDataFileSystem::OnGetFileCompleteForUpdateFileByEntry(
    const FileOperationCallback& callback,
    const std::string& md5,
    int64 file_size,
    const FilePath& cache_file_path,
    GDataEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!entry || !entry->AsGDataFile()) {
    if (!callback.is_null())
      callback.Run(GDATA_FILE_ERROR_NOT_FOUND);
    return;
  }
  GDataFile* file = entry->AsGDataFile();

  uploader_->UploadExistingFile(
      file->upload_url(),
      file->GetFilePath(),
      cache_file_path,
      file_size,
      file->content_mime_type(),
      base::Bind(&GDataFileSystem::OnUpdatedFileUploaded,
                 ui_weak_ptr_,
                 callback));
}
