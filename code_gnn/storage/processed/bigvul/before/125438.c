GDataFileError GDataFileSystem::RemoveEntryFromGData(
    const FilePath& file_path, std::string* resource_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  resource_id->clear();

  GDataEntry* entry = directory_service_->FindEntryByPathSync(file_path);

  if (!entry)
    return GDATA_FILE_ERROR_NOT_FOUND;

  if (!entry->parent())
    return GDATA_FILE_ERROR_ACCESS_DENIED;

  if (entry->AsGDataFile())
    *resource_id = entry->AsGDataFile()->resource_id();

  GDataDirectory* parent_dir = entry->parent();
  parent_dir->RemoveEntry(entry);

  OnDirectoryChanged(parent_dir->GetFilePath());
  return GDATA_FILE_OK;
}
