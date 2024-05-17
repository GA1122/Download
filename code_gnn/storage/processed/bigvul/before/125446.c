void GDataFileSystem::RequestDirectoryRefresh(const FilePath& file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(
      base::Bind(&GDataFileSystem::RequestDirectoryRefreshOnUIThread,
                 ui_weak_ptr_,
                 file_path));
}
