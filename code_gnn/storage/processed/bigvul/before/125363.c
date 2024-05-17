void GDataFileSystem::GetEntryInfoByPath(const FilePath& file_path,
                                         const GetEntryInfoCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(
      base::Bind(&GDataFileSystem::GetEntryInfoByPathAsyncOnUIThread,
                 ui_weak_ptr_,
                 file_path,
                 CreateRelayCallback(callback)));
}
