void GDataFileSystem::OnGetFileCompleteForOpenFile(
    const OpenFileCallback& callback,
    const GetFileCompleteForOpenParams& entry_proto,
    GDataFileError error,
    const FilePath& file_path,
    const std::string& mime_type,
    GDataFileType file_type) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error, FilePath());
    return;
  }

  DCHECK_EQ(REGULAR_FILE, file_type);

  cache_->MarkDirtyOnUIThread(
      entry_proto.resource_id,
      entry_proto.md5,
      base::Bind(&GDataFileSystem::OnMarkDirtyInCacheCompleteForOpenFile,
                 ui_weak_ptr_,
                 callback));
}
