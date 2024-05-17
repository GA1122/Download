void AppCacheUpdateJob::NotifyAllFinalProgress() {
  DCHECK(url_file_list_.size() == url_fetches_completed_);
  NotifyAllProgress(GURL());
}
