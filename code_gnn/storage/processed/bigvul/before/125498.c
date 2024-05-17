void GDataDirectoryService::FindEntryByPathAndRunSync(
    const FilePath& search_file_path,
    const FindEntryCallback& callback) {
  GDataEntry* entry = FindEntryByPathSync(search_file_path);
  callback.Run(entry ? GDATA_FILE_OK : GDATA_FILE_ERROR_NOT_FOUND, entry);
}
