void GDataFileSystem::ReadDirectoryByPath(
    const FilePath& file_path,
    const ReadDirectoryWithSettingCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(
      base::Bind(&GDataFileSystem::ReadDirectoryByPathAsyncOnUIThread,
                 ui_weak_ptr_,
                 file_path,
                 CreateRelayCallback(callback)));
}
