void GDataFileSystem::OnGetFileCompleteForUpdateFile(
    const FileOperationCallback& callback,
    GDataFileError error,
    const std::string& resource_id,
    const std::string& md5,
    const FilePath& cache_file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error);
    return;
  }

  GDataFileError* get_size_error = new GDataFileError(GDATA_FILE_ERROR_FAILED);
  int64* file_size = new int64(-1);
  util::PostBlockingPoolSequencedTaskAndReply(
      FROM_HERE,
      blocking_task_runner_,
      base::Bind(&GetLocalFileSizeOnBlockingPool,
                 cache_file_path,
                 get_size_error,
                 file_size),
      base::Bind(&GDataFileSystem::OnGetFileSizeCompleteForUpdateFile,
                 ui_weak_ptr_,
                 callback,
                 resource_id,
                 md5,
                 cache_file_path,
                 base::Owned(get_size_error),
                 base::Owned(file_size)));
}
