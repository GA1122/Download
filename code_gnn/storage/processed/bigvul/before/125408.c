void GDataFileSystem::OnGetEntryInfoCompleteForOpenFile(
    const FilePath& file_path,
    const OpenFileCallback& callback,
    GDataFileError error,
    scoped_ptr<GDataEntryProto> entry_proto) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (entry_proto.get() && !entry_proto->has_file_specific_info())
    error = GDATA_FILE_ERROR_NOT_FOUND;

  if (error == GDATA_FILE_OK) {
    if (entry_proto->file_specific_info().file_md5().empty() ||
        entry_proto->file_specific_info().is_hosted_document()) {
      error = GDATA_FILE_ERROR_INVALID_OPERATION;
    }
  }

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error, FilePath());
    return;
  }

  DCHECK(!entry_proto->resource_id().empty());
  GetResolvedFileByPath(
      file_path,
      base::Bind(&GDataFileSystem::OnGetFileCompleteForOpenFile,
                 ui_weak_ptr_,
                 callback,
                 GetFileCompleteForOpenParams(
                     entry_proto->resource_id(),
                     entry_proto->file_specific_info().file_md5())),
      GetDownloadDataCallback(),
      error,
      entry_proto.get());
}
