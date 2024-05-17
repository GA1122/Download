void GDataFileSystem::StartFileUploadOnUIThread(
    const StartFileUploadParams& params,
    GDataFileError* error,
    int64* file_size,
    std::string* content_type) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(error);
  DCHECK(file_size);
  DCHECK(content_type);

  if (*error != GDATA_FILE_OK) {
    if (!params.callback.is_null())
      params.callback.Run(*error);

    return;
  }

  directory_service_->GetEntryInfoByPath(
      params.remote_file_path.DirName(),
      base::Bind(
          &GDataFileSystem::StartFileUploadOnUIThreadAfterGetEntryInfo,
          ui_weak_ptr_,
          params,
          *file_size,
          *content_type));
}
