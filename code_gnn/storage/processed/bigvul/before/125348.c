void GDataFileSystem::CreateDirectory(
    const FilePath& directory_path,
    bool is_exclusive,
    bool is_recursive,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(base::Bind(&GDataFileSystem::CreateDirectoryOnUIThread,
                               ui_weak_ptr_,
                               directory_path,
                               is_exclusive,
                               is_recursive,
                               CreateRelayCallback(callback)));
}
