void GDataFileSystem::OnGetEntryInfoForCreateFile(
    const FilePath& file_path,
    bool is_exclusive,
    const FileOperationCallback& callback,
    GDataFileError result,
    GDataEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (result != GDATA_FILE_ERROR_NOT_FOUND &&
      result != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(result);
    return;
  }

  if (result == GDATA_FILE_OK) {
    if (is_exclusive ||
        !entry->AsGDataFile() ||
        entry->AsGDataFile()->is_hosted_document()) {
      if (!callback.is_null())
        callback.Run(GDATA_FILE_ERROR_EXISTS);
      return;
    }

    if (!callback.is_null())
      callback.Run(GDATA_FILE_OK);
    return;
  }

  TransferRegularFile(FilePath(kEmptyFilePath), file_path, callback);
}
