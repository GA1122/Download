void RunCacheOperationCallback(const CacheOperationCallback& callback,
                               base::PlatformFileError* error,
                               const std::string& resource_id,
                               const std::string& md5) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(error);

  if (!callback.is_null())
    callback.Run(*error, resource_id, md5);
}
