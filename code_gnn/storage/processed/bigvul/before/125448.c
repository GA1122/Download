void GDataFileSystem::RequestDirectoryRefreshOnUIThread(
    const FilePath& file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  directory_service_->GetEntryInfoByPath(
      file_path,
      base::Bind(
          &GDataFileSystem::RequestDirectoryRefreshOnUIThreadAfterGetEntryInfo,
          ui_weak_ptr_,
          file_path));
}
