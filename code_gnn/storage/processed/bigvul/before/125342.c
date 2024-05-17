void GDataFileSystem::CloseFileOnUIThread(
    const FilePath& file_path,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (open_files_.find(file_path) == open_files_.end()) {
    MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(callback, GDATA_FILE_ERROR_NOT_FOUND));
    return;
  }

  GetEntryInfoByPathAsyncOnUIThread(
      file_path,
      base::Bind(&GDataFileSystem::OnGetEntryInfoCompleteForCloseFile,
                 ui_weak_ptr_,
                 file_path,
                 base::Bind(&GDataFileSystem::OnCloseFileFinished,
                            ui_weak_ptr_,
                            file_path,
                            callback)));
}
