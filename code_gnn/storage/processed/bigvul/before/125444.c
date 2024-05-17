void GDataFileSystem::RenameAfterGetEntryInfo(
    const FilePath& file_path,
    const FilePath::StringType& new_name,
    const FileMoveCallback& callback,
    GDataFileError error,
    scoped_ptr<GDataEntryProto> entry_proto) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error, file_path);
    return;
  }
  DCHECK(entry_proto.get());

  FilePath::StringType file_name = new_name;
  if (entry_proto->has_file_specific_info() &&
      entry_proto->file_specific_info().is_hosted_document()) {
    FilePath new_file(file_name);
    if (new_file.Extension() ==
        entry_proto->file_specific_info().document_extension()) {
      file_name = new_file.RemoveExtension().value();
    }
  }

  documents_service_->RenameResource(
      GURL(entry_proto->edit_url()),
      file_name,
      base::Bind(&GDataFileSystem::RenameFileOnFileSystem,
                 ui_weak_ptr_,
                 file_path,
                 file_name,
                 callback));
}
