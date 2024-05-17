void GDataFileSystem::GetFileByEntryOnUIThread(
    const GetFileCallback& get_file_callback,
    const GetDownloadDataCallback& get_download_data_callback,
    GDataEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  FilePath file_path;
  if (entry) {
    GDataFile* file = entry->AsGDataFile();
    if (file)
      file_path = file->GetFilePath();
  }

  if (file_path.empty()) {
    if (!get_file_callback.is_null()) {
      base::MessageLoopProxy::current()->PostTask(
          FROM_HERE,
          base::Bind(get_file_callback,
                     GDATA_FILE_ERROR_NOT_FOUND,
                     FilePath(),
                     std::string(),
                     REGULAR_FILE));
    }
    return;
  }

  GetFileByPath(file_path, get_file_callback, get_download_data_callback);
}
