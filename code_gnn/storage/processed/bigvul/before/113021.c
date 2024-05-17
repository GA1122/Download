static void DeleteDownloadedFile(const FilePath& path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));

  if (!file_util::DirectoryExists(path))
    file_util::Delete(path, false);
}
