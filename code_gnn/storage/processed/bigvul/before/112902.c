void RunSetMountedStateCallback(const SetMountedStateCallback& callback,
                                base::PlatformFileError* error,
                                FilePath* cache_file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(error);
  DCHECK(cache_file_path);

  if (!callback.is_null())
    callback.Run(*error, *cache_file_path);
}
