void GDataFileSystem::StartDownloadFileIfEnoughSpace(
    const GetFileFromCacheParams& params,
    const GURL& content_url,
    const FilePath& cache_file_path,
    bool* has_enough_space) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!*has_enough_space) {
    if (!params.get_file_callback.is_null()) {
      params.get_file_callback.Run(GDATA_FILE_ERROR_NO_SPACE,
                                   cache_file_path,
                                   params.mime_type,
                                   REGULAR_FILE);
    }
    return;
  }

  documents_service_->DownloadFile(
      params.virtual_file_path,
      params.local_tmp_path,
      content_url,
      base::Bind(&GDataFileSystem::OnFileDownloaded,
                 ui_weak_ptr_,
                 params),
      params.get_download_data_callback);
}
