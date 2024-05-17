void GDataFileSystem::OnReadDirectory(
    const ReadDirectoryWithSettingCallback& callback,
    GDataFileError error,
    GDataEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error,
                   hide_hosted_docs_,
                   scoped_ptr<GDataEntryProtoVector>());
    return;
  }
  DCHECK(entry);

  GDataDirectory* directory = entry->AsGDataDirectory();
  if (!directory) {
    if (!callback.is_null())
      callback.Run(GDATA_FILE_ERROR_NOT_FOUND,
                   hide_hosted_docs_,
                   scoped_ptr<GDataEntryProtoVector>());
    return;
  }

  scoped_ptr<GDataEntryProtoVector> entries(directory->ToProtoVector());

  if (!callback.is_null())
    callback.Run(GDATA_FILE_OK, hide_hosted_docs_, entries.Pass());
}
