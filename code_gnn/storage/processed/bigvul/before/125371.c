void GDataFileSystem::GetFileByResourceIdOnUIThread(
    const std::string& resource_id,
    const GetFileCallback& get_file_callback,
    const GetDownloadDataCallback& get_download_data_callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  directory_service_->GetEntryByResourceIdAsync(resource_id,
      base::Bind(&GDataFileSystem::GetFileByEntryOnUIThread,
                 ui_weak_ptr_,
                 get_file_callback,
                 get_download_data_callback));
}
