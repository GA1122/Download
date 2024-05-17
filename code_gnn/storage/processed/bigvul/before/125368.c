void GDataFileSystem::GetFileByPath(
    const FilePath& file_path,
    const GetFileCallback& get_file_callback,
    const GetDownloadDataCallback& get_download_data_callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(
      base::Bind(&GDataFileSystem::GetFileByPathOnUIThread,
                 ui_weak_ptr_,
                 file_path,
                 CreateRelayCallback(get_file_callback),
                 CreateRelayCallback(get_download_data_callback)));
}
