void GDataFileSystem::RenameFileOnFileSystem(
    const FilePath& file_path,
    const FilePath::StringType& new_name,
    const FileMoveCallback& callback,
    GDataErrorCode status,
    const GURL& document_url) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  const GDataFileError error = util::GDataToGDataFileError(status);
  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error, FilePath());
    return;
  }

  GDataEntry* entry = directory_service_->FindEntryByPathSync(file_path);
  if (!entry) {
    if (!callback.is_null())
      callback.Run(GDATA_FILE_ERROR_NOT_FOUND, FilePath());
    return;
  }

  DCHECK(entry->parent());
  entry->set_title(new_name);
  directory_service_->MoveEntryToDirectory(
      entry->parent()->GetFilePath(),
      entry,
      base::Bind(&GDataFileSystem::OnMoveEntryToDirectoryWithFileMoveCallback,
                 ui_weak_ptr_,
                 callback));
}
