void StoragePartitionImpl::ClearHttpAndMediaCaches(
    const base::Time begin,
    const base::Time end,
    const base::Callback<bool(const GURL&)>& url_matcher,
    base::OnceClosure callback) {
  if (url_matcher.is_null()) {
    StoragePartitionHttpCacheDataRemover::CreateForRange(this, begin, end)
        ->Remove(std::move(callback));
  } else {
    StoragePartitionHttpCacheDataRemover::CreateForURLsAndRange(
        this, url_matcher, begin, end)
        ->Remove(std::move(callback));
  }
}
