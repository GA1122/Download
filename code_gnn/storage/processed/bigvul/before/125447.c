void GDataFileSystem::RequestDirectoryRefreshByEntry(
    const FilePath& directory_path,
    const std::string& directory_resource_id,
    const FileResourceIdMap& file_map,
    GDataEntry* directory_entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!directory_entry || !directory_entry->AsGDataDirectory()) {
    LOG(ERROR) << "Directory entry is gone: " << directory_path.value()
               << ": " << directory_resource_id;
    return;
  }
  GDataDirectory* directory = directory_entry->AsGDataDirectory();

  directory->RemoveChildFiles();
  for (FileResourceIdMap::const_iterator it = file_map.begin();
       it != file_map.end(); ++it) {
    scoped_ptr<GDataEntry> entry(it->second);
    if (!entry->AsGDataFile())
      continue;
    directory->AddEntry(entry.release());
  }

  OnDirectoryChanged(directory_path);
  DVLOG(1) << "Directory refreshed: " << directory_path.value();
}
