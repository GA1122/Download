void GDataCache::OnUnpinned(base::PlatformFileError* error,
                            const std::string& resource_id,
                            const std::string& md5,
                            const CacheOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(error);

  if (!callback.is_null())
    callback.Run(*error, resource_id, md5);

  if (*error == base::PLATFORM_FILE_OK)
    FOR_EACH_OBSERVER(Observer, observers_, OnCacheUnpinned(resource_id, md5));

  bool* has_enough_space = new bool(false);
  pool_->GetSequencedTaskRunner(sequence_token_)->PostTask(
      FROM_HERE,
      base::Bind(&GDataCache::FreeDiskSpaceIfNeededFor,
                 base::Unretained(this),
                 0,
                 base::Owned(has_enough_space)));
}
