void GDataFileSystem::GetResolvedFileByPath(
    const FilePath& file_path,
    const GetFileCallback& get_file_callback,
    const GetDownloadDataCallback& get_download_data_callback,
    GDataFileError error,
    const GDataEntryProto* entry_proto) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (entry_proto && !entry_proto->has_file_specific_info())
    error = GDATA_FILE_ERROR_NOT_FOUND;

  if (error != GDATA_FILE_OK) {
    if (!get_file_callback.is_null()) {
      MessageLoop::current()->PostTask(
          FROM_HERE,
          base::Bind(get_file_callback,
                     GDATA_FILE_ERROR_NOT_FOUND,
                     FilePath(),
                     std::string(),
                     REGULAR_FILE));
    }
    return;
  }

  if (entry_proto->file_specific_info().is_hosted_document()) {
    GDataFileError* error =
        new GDataFileError(GDATA_FILE_OK);
    FilePath* temp_file_path = new FilePath;
    std::string* mime_type = new std::string;
    GDataFileType* file_type = new GDataFileType(REGULAR_FILE);
    util::PostBlockingPoolSequencedTaskAndReply(
        FROM_HERE,
        blocking_task_runner_,
        base::Bind(&CreateDocumentJsonFileOnBlockingPool,
                   cache_->GetCacheDirectoryPath(
                       GDataCache::CACHE_TYPE_TMP_DOCUMENTS),
                   GURL(entry_proto->file_specific_info().alternate_url()),
                   entry_proto->resource_id(),
                   error,
                   temp_file_path,
                   mime_type,
                   file_type),
        base::Bind(&RunGetFileCallbackHelper,
                   get_file_callback,
                   base::Owned(error),
                   base::Owned(temp_file_path),
                   base::Owned(mime_type),
                   base::Owned(file_type)));
    return;
  }

  FilePath local_tmp_path = cache_->GetCacheFilePath(
      entry_proto->resource_id(),
      entry_proto->file_specific_info().file_md5(),
      GDataCache::CACHE_TYPE_TMP,
      GDataCache::CACHED_FILE_FROM_SERVER);
  cache_->GetFileOnUIThread(
      entry_proto->resource_id(),
      entry_proto->file_specific_info().file_md5(),
      base::Bind(
          &GDataFileSystem::OnGetFileFromCache,
          ui_weak_ptr_,
          GetFileFromCacheParams(
              file_path,
              local_tmp_path,
              GURL(entry_proto->content_url()),
              entry_proto->resource_id(),
              entry_proto->file_specific_info().file_md5(),
              entry_proto->file_specific_info().content_mime_type(),
              get_file_callback,
              get_download_data_callback)));
}
