void GDataFileSystem::OnFileDownloadedAndSpaceChecked(
    const GetFileFromCacheParams& params,
    GDataErrorCode status,
    const GURL& content_url,
    const FilePath& downloaded_file_path,
    bool* has_enough_space) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  GDataFileError error = util::GDataToGDataFileError(status);

  if (error == GDATA_FILE_OK) {
    if (*has_enough_space) {
      cache_->StoreOnUIThread(
          params.resource_id,
          params.md5,
          downloaded_file_path,
          GDataCache::FILE_OPERATION_MOVE,
          base::Bind(&GDataFileSystem::OnDownloadStoredToCache,
                     ui_weak_ptr_));
    } else {
      util::PostBlockingPoolSequencedTask(
          FROM_HERE,
          blocking_task_runner_,
          base::Bind(base::IgnoreResult(&file_util::Delete),
                     downloaded_file_path,
                     false  ));
      error = GDATA_FILE_ERROR_NO_SPACE;
    }
  }

  if (!params.get_file_callback.is_null()) {
    params.get_file_callback.Run(error,
                                 downloaded_file_path,
                                 params.mime_type,
                                 REGULAR_FILE);
  }
}
