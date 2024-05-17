void BackgroundLoaderOffliner::ResetState() {
  pending_request_.reset();
  snapshot_controller_->Stop();
  base::ThreadTaskRunnerHandle::Get()->DeleteSoon(
      FROM_HERE, snapshot_controller_.release());
  page_load_state_ = SUCCESS;
  network_bytes_ = 0LL;
  is_low_bar_met_ = false;
  did_snapshot_on_last_retry_ = false;
  content::WebContentsObserver::Observe(nullptr);
  loader_.reset();

  for (int i = 0; i < ResourceDataType::RESOURCE_DATA_TYPE_COUNT; ++i) {
    stats_[i].requested = 0;
    stats_[i].completed = 0;
  }
}
