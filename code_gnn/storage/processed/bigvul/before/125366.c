void GDataFileSystem::GetEntryInfoByResourceIdOnUIThread(
    const std::string& resource_id,
    const GetEntryInfoWithFilePathCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  directory_service_->GetEntryByResourceIdAsync(resource_id,
      base::Bind(&GDataFileSystem::GetEntryInfoByEntryOnUIThread,
                 ui_weak_ptr_,
                 callback));
}
