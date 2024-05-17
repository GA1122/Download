void GDataCache::GetCacheEntryOnUIThread(
    const std::string& resource_id,
    const std::string& md5,
    const GetCacheEntryCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  bool* success = new bool(false);
  GDataCache::CacheEntry* cache_entry = new GDataCache::CacheEntry;
  pool_->GetSequencedTaskRunner(sequence_token_)->PostTaskAndReply(
      FROM_HERE,
      base::Bind(&GDataCache::GetCacheEntryHelper,
                 base::Unretained(this),
                 resource_id,
                 md5,
                 success,
                 cache_entry),
      base::Bind(&RunGetCacheEntryCallback,
                 callback,
                 base::Owned(success),
                 base::Owned(cache_entry)));
}
