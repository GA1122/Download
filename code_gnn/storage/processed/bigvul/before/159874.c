void TopSitesImpl::GetMostVisitedURLs(
    const GetMostVisitedURLsCallback& callback,
    bool include_forced_urls) {
  MostVisitedURLList filtered_urls;
  {
    base::AutoLock lock(lock_);
    if (!loaded_) {
      pending_callbacks_.push_back(
          base::Bind(&RunOrPostGetMostVisitedURLsCallback,
                     base::RetainedRef(base::ThreadTaskRunnerHandle::Get()),
                     include_forced_urls, callback));
      return;
    }
    if (include_forced_urls) {
      filtered_urls = thread_safe_cache_->top_sites();
    } else {
      filtered_urls.assign(thread_safe_cache_->top_sites().begin() +
                              thread_safe_cache_->GetNumForcedURLs(),
                           thread_safe_cache_->top_sites().end());
    }
  }
  callback.Run(filtered_urls);
}
