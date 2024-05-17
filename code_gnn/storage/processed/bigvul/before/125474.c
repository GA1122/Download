void GDataFileSystem::TransferFileFromRemoteToLocal(
    const FilePath& remote_src_file_path,
    const FilePath& local_dest_file_path,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  GetFileByPath(remote_src_file_path,
      base::Bind(&GDataFileSystem::OnGetFileCompleteForTransferFile,
                 ui_weak_ptr_,
                 local_dest_file_path,
                 callback),
      GetDownloadDataCallback());
}
