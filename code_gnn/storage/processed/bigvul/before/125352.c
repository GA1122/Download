void GDataFileSystem::CreateFile(const FilePath& file_path,
                                 bool is_exclusive,
                                 const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(base::Bind(&GDataFileSystem::CreateFileOnUIThread,
                               ui_weak_ptr_,
                               file_path,
                               is_exclusive,
                               CreateRelayCallback(callback)));
}
