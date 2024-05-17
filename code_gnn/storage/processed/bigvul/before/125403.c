void GDataFileSystem::OnGetCacheFilePathCompleteForCloseFile(
    const FilePath& file_path,
    const FileOperationCallback& callback,
    GDataFileError error,
    const std::string& resource_id,
    const std::string& md5,
    const FilePath& local_cache_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error);
    return;
  }

  base::PlatformFileInfo* file_info = new base::PlatformFileInfo;
  bool* get_file_info_result = new bool(false);
  util::PostBlockingPoolSequencedTaskAndReply(
      FROM_HERE,
      blocking_task_runner_,
      base::Bind(&GetFileInfoOnBlockingPool,
                 local_cache_path,
                 base::Unretained(file_info),
                 base::Unretained(get_file_info_result)),
      base::Bind(&GDataFileSystem::OnGetModifiedFileInfoCompleteForCloseFile,
                 ui_weak_ptr_,
                 file_path,
                 base::Owned(file_info),
                 base::Owned(get_file_info_result),
                 callback));
}
