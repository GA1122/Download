void DownloadManagerImpl::OnHistoryQueryComplete(
    base::OnceClosure load_history_downloads_cb) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (base::FeatureList::IsEnabled(
          download::features::kDownloadDBForNewDownloads) &&
      !in_progress_cache_initialized_) {
    load_history_downloads_cb_ = std::move(load_history_downloads_cb);
  } else {
    std::move(load_history_downloads_cb).Run();
  }
}
