void GDataFileSystem::GetFileByPathOnUIThread(
    const FilePath& file_path,
    const GetFileCallback& get_file_callback,
    const GetDownloadDataCallback& get_download_data_callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  directory_service_->GetEntryInfoByPath(
      file_path,
      base::Bind(&GDataFileSystem::OnGetEntryInfoCompleteForGetFileByPath,
                 ui_weak_ptr_,
                 file_path,
                 CreateRelayCallback(get_file_callback),
                 CreateRelayCallback(get_download_data_callback)));
}
