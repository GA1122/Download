void GDataDirectoryService::ReadDirectoryByPath(
    const FilePath& path,
    const ReadDirectoryCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  scoped_ptr<GDataEntryProtoVector> entries;
  GDataFileError error = GDATA_FILE_ERROR_FAILED;

  GDataEntry* entry = FindEntryByPathSync(path);
  if (entry && entry->AsGDataDirectory()) {
    entries = entry->AsGDataDirectory()->ToProtoVector();
    error = GDATA_FILE_OK;
  } else if (entry && !entry->AsGDataDirectory()) {
    error = GDATA_FILE_ERROR_NOT_A_DIRECTORY;
  } else {
    error = GDATA_FILE_ERROR_NOT_FOUND;
  }

  base::MessageLoopProxy::current()->PostTask(
      FROM_HERE,
      base::Bind(callback, error, base::Passed(&entries)));
}
