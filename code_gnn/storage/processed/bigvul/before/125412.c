void GDataFileSystem::OnGetFileCompleteForTransferFile(
    const FilePath& local_dest_file_path,
    const FileOperationCallback& callback,
    GDataFileError error,
    const FilePath& local_file_path,
    const std::string& unused_mime_type,
    GDataFileType file_type) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  if (error != GDATA_FILE_OK) {
    callback.Run(error);
    return;
  }

  GDataFileError* copy_file_error =
      new GDataFileError(GDATA_FILE_OK);
  util::PostBlockingPoolSequencedTaskAndReply(
      FROM_HERE,
      blocking_task_runner_,
      base::Bind(&CopyLocalFileOnBlockingPool,
                 local_file_path,
                 local_dest_file_path,
                 copy_file_error),
      base::Bind(&RunFileOperationCallbackHelper,
                 callback,
                 base::Owned(copy_file_error)));
}
