void GDataFileSystem::MoveEntryFromRootDirectory(
    const FilePath& dir_path,
    const FileOperationCallback& callback,
    GDataFileError error,
    const FilePath& file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());
  DCHECK_EQ(kGDataRootDirectory, file_path.DirName().value());

  GDataEntry* entry = directory_service_->FindEntryByPathSync(file_path);
  GDataEntry* dir_entry = directory_service_->FindEntryByPathSync(dir_path);
  if (error == GDATA_FILE_OK) {
    if (!entry || !dir_entry) {
      error = GDATA_FILE_ERROR_NOT_FOUND;
    } else {
      if (!dir_entry->AsGDataDirectory())
        error = GDATA_FILE_ERROR_NOT_A_DIRECTORY;
    }
  }

  if (error != GDATA_FILE_OK ||
      dir_entry->resource_id() == kGDataRootDirectoryResourceId) {
    callback.Run(error);
    return;
  }

  documents_service_->AddResourceToDirectory(
      dir_entry->content_url(),
      entry->edit_url(),
      base::Bind(&GDataFileSystem::OnMoveEntryFromRootDirectoryCompleted,
                 ui_weak_ptr_,
                 callback,
                 file_path,
                 dir_path));
}
