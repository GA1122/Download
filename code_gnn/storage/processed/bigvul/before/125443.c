void GDataFileSystem::Rename(const FilePath& file_path,
                             const FilePath::StringType& new_name,
                             const FileMoveCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  if (file_path.BaseName().value() == new_name) {
    callback.Run(GDATA_FILE_OK, file_path);
    return;
  }

  directory_service_->GetEntryInfoByPath(
      file_path,
      base::Bind(
          &GDataFileSystem::RenameAfterGetEntryInfo,
          ui_weak_ptr_,
          file_path,
          new_name,
          callback));
}
