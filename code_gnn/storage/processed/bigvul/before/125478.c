void GDataFileSystem::UpdateFileByResourceId(
    const std::string& resource_id,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  RunTaskOnUIThread(
      base::Bind(&GDataFileSystem::UpdateFileByResourceIdOnUIThread,
                 ui_weak_ptr_,
                 resource_id,
                 CreateRelayCallback(callback)));
}
