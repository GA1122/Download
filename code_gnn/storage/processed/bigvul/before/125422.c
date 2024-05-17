void GDataFileSystem::OnOpenFileFinished(const FilePath& file_path,
                                         const OpenFileCallback& callback,
                                         GDataFileError result,
                                         const FilePath& cache_file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (result != GDATA_FILE_OK)
    open_files_.erase(file_path);

  if (!callback.is_null())
    callback.Run(result, cache_file_path);
}
