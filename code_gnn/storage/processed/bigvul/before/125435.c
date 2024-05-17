void GDataFileSystem::RemoveEntryFromDirectory(
    const FilePath& dir_path,
    const FileMoveCallback& callback,
    GDataFileError error,
    const FilePath& file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  GDataEntry* entry = directory_service_->FindEntryByPathSync(file_path);
  GDataEntry* dir = directory_service_->FindEntryByPathSync(dir_path);
  if (error == GDATA_FILE_OK) {
    if (!entry || !dir) {
      error = GDATA_FILE_ERROR_NOT_FOUND;
    } else {
      if (!dir->AsGDataDirectory())
        error = GDATA_FILE_ERROR_NOT_A_DIRECTORY;
    }
  }

  if (error != GDATA_FILE_OK ||
      dir->resource_id() == kGDataRootDirectoryResourceId) {
    if (!callback.is_null()) {
      MessageLoop::current()->PostTask(FROM_HERE,
          base::Bind(callback, error, file_path));
    }
    return;
  }

  documents_service_->RemoveResourceFromDirectory(
      dir->content_url(),
      entry->edit_url(),
      entry->resource_id(),
      base::Bind(&GDataFileSystem::RemoveEntryFromDirectoryOnFileSystem,
                 ui_weak_ptr_,
                 callback,
                 file_path,
                 dir_path));
}
