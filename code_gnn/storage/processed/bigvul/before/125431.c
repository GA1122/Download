void GDataFileSystem::OpenFileOnUIThread(const FilePath& file_path,
                                         const OpenFileCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (open_files_.find(file_path) != open_files_.end()) {
    MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(callback, GDATA_FILE_ERROR_IN_USE, FilePath()));
    return;
  }
  open_files_.insert(file_path);

  directory_service_->GetEntryInfoByPath(
      file_path,
      base::Bind(&GDataFileSystem::OnGetEntryInfoCompleteForOpenFile,
                 ui_weak_ptr_,
                 file_path,
                 base::Bind(&GDataFileSystem::OnOpenFileFinished,
                            ui_weak_ptr_,
                            file_path,
                            callback)));
}
