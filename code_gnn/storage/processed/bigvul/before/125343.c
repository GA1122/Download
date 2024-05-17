void GDataFileSystem::Copy(const FilePath& src_file_path,
                           const FilePath& dest_file_path,
                           const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(!callback.is_null());

  RunTaskOnUIThread(base::Bind(&GDataFileSystem::CopyOnUIThread,
                               ui_weak_ptr_,
                               src_file_path,
                               dest_file_path,
                               CreateRelayCallback(callback)));
}
