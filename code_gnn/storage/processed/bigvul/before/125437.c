GDataFileError GDataFileSystem::RemoveEntryFromFileSystem(
    const FilePath& file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  std::string resource_id;
  GDataFileError error = RemoveEntryFromGData(file_path, &resource_id);
  if (error != GDATA_FILE_OK)
    return error;

  if (!resource_id.empty())
    cache_->RemoveOnUIThread(resource_id, CacheOperationCallback());

  return GDATA_FILE_OK;
}
