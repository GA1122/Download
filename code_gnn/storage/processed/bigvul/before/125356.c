void GDataFileSystem::FindEntryByPathSyncOnUIThread(
    const FilePath& search_file_path,
    const FindEntryCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  directory_service_->FindEntryByPathAndRunSync(search_file_path, callback);
}
