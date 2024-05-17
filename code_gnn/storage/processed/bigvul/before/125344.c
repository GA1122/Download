void GDataFileSystem::CopyDocumentToDirectory(
    const FilePath& dir_path,
    const std::string& resource_id,
    const FilePath::StringType& new_name,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  documents_service_->CopyDocument(resource_id, new_name,
      base::Bind(&GDataFileSystem::OnCopyDocumentCompleted,
                 ui_weak_ptr_,
                 dir_path,
                 callback));
}
