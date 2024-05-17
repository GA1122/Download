void RunGetFileFromCacheCallback(const GetFileFromCacheCallback& callback,
                                 base::PlatformFileError* error,
                                 const std::string& resource_id,
                                 const std::string& md5,
                                 FilePath* cache_file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(error);
  DCHECK(cache_file_path);

  if (!callback.is_null())
    callback.Run(*error, resource_id, md5, *cache_file_path);
}
