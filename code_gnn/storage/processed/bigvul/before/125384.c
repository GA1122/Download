void GDataFileSystem::MoveOnUIThread(const FilePath& src_file_path,
                                     const FilePath& dest_file_path,
                                     const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  directory_service_->GetEntryInfoPairByPaths(
      src_file_path,
      dest_file_path.DirName(),
      base::Bind(&GDataFileSystem::MoveOnUIThreadAfterGetEntryInfoPair,
                 ui_weak_ptr_,
                 dest_file_path,
                 callback));
}
