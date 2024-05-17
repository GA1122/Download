void GDataFileSystem::CloseFile(const FilePath& file_path,
                                const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(base::Bind(&GDataFileSystem::CloseFileOnUIThread,
                               ui_weak_ptr_,
                               file_path,
                               CreateRelayCallback(callback)));
}
