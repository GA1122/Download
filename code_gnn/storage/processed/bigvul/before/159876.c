bool TopSitesImpl::GetPageThumbnailScore(const GURL& url,
                                         ThumbnailScore* score) {
  base::AutoLock lock(lock_);
  return thread_safe_cache_->GetPageThumbnailScore(url, score);
}
