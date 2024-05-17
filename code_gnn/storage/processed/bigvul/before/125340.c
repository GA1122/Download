void GDataFileSystem::CheckLocalModificationAndRunAfterGetFileInfo(
    scoped_ptr<GDataEntryProto> entry_proto,
    const GetEntryInfoCallback& callback,
    base::PlatformFileInfo* file_info,
    bool* get_file_info_result) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!*get_file_info_result) {
    if (!callback.is_null())
      callback.Run(GDATA_FILE_ERROR_NOT_FOUND, scoped_ptr<GDataEntryProto>());
    return;
  }

  PlatformFileInfoProto entry_file_info;
  GDataEntry::ConvertPlatformFileInfoToProto(*file_info, &entry_file_info);
  *entry_proto->mutable_file_info() = entry_file_info;
  if (!callback.is_null())
    callback.Run(GDATA_FILE_OK, entry_proto.Pass());
}
