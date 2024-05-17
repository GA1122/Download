void GDataFileSystem::OpenFile(const FilePath& file_path,
                               const OpenFileCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(base::Bind(&GDataFileSystem::OpenFileOnUIThread,
                               ui_weak_ptr_,
                               file_path,
                               CreateRelayCallback(callback)));
}
