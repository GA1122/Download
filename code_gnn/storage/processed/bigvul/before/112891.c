void GDataCache::OnPinned(base::PlatformFileError* error,
                          const std::string& resource_id,
                          const std::string& md5,
                          const CacheOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(error);

  if (!callback.is_null())
    callback.Run(*error, resource_id, md5);

  if (*error == base::PLATFORM_FILE_OK)
    FOR_EACH_OBSERVER(Observer, observers_, OnCachePinned(resource_id, md5));
}
