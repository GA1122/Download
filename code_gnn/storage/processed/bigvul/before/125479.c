void GDataFileSystem::UpdateFileByResourceIdOnUIThread(
    const std::string& resource_id,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  directory_service_->GetEntryByResourceIdAsync(resource_id,
      base::Bind(&GDataFileSystem::UpdateFileByEntryOnUIThread,
                 ui_weak_ptr_,
                 callback));
}
