void GDataFileSystem::TransferFileFromLocalToRemote(
    const FilePath& local_src_file_path,
    const FilePath& remote_dest_file_path,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  directory_service_->GetEntryInfoByPath(
      remote_dest_file_path.DirName(),
      base::Bind(
          &GDataFileSystem::TransferFileFromLocalToRemoteAfterGetEntryInfo,
          ui_weak_ptr_,
          local_src_file_path,
          remote_dest_file_path,
          callback));
}
