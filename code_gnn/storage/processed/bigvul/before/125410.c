void GDataFileSystem::OnGetFileCompleteForCopy(
    const FilePath& remote_dest_file_path,
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

  DCHECK_EQ(REGULAR_FILE, file_type);
  BrowserThread::PostTask(
      BrowserThread::UI,
      FROM_HERE,
      base::Bind(&GDataFileSystem::TransferRegularFile,
                 ui_weak_ptr_,
                 local_file_path, remote_dest_file_path,
                 base::Bind(OnTransferRegularFileCompleteForCopy,
                            callback,
                            base::MessageLoopProxy::current())));
}
