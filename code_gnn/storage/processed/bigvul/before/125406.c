void GDataFileSystem::OnGetEntryInfoCompleteForCloseFile(
    const FilePath& file_path,
    const FileOperationCallback& callback,
    GDataFileError error,
    scoped_ptr<GDataEntryProto> entry_proto) {
  if (entry_proto.get() && !entry_proto->has_file_specific_info())
    error = GDATA_FILE_ERROR_NOT_FOUND;

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error);
    return;
  }

  cache_->GetFileOnUIThread(
      entry_proto->resource_id(),
      entry_proto->file_specific_info().file_md5(),
      base::Bind(&GDataFileSystem::OnGetCacheFilePathCompleteForCloseFile,
                 ui_weak_ptr_,
                 file_path,
                 callback));
}
