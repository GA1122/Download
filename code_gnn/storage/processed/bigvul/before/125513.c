void GDataDirectoryService::MoveEntryToDirectory(
    const FilePath& directory_path,
    GDataEntry* entry,
    const FileMoveCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(entry);

  if (entry->parent())
    entry->parent()->RemoveChild(entry);

  GDataEntry* destination = FindEntryByPathSync(directory_path);
  FilePath moved_file_path;
  GDataFileError error = GDATA_FILE_ERROR_FAILED;
  if (!destination) {
    error = GDATA_FILE_ERROR_NOT_FOUND;
  } else if (!destination->AsGDataDirectory()) {
    error = GDATA_FILE_ERROR_NOT_A_DIRECTORY;
  } else {
    destination->AsGDataDirectory()->AddEntry(entry);
    moved_file_path = entry->GetFilePath();
    error = GDATA_FILE_OK;
  }
  if (!callback.is_null()) {
    base::MessageLoopProxy::current()->PostTask(
        FROM_HERE, base::Bind(callback, error, moved_file_path));
  }
}
