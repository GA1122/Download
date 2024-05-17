BackgroundLoaderOffliner::BackgroundLoaderOffliner(
    content::BrowserContext* browser_context,
    const OfflinerPolicy* policy,
    OfflinePageModel* offline_page_model,
    std::unique_ptr<LoadTerminationListener> load_termination_listener)
    : browser_context_(browser_context),
      offline_page_model_(offline_page_model),
      policy_(policy),
      load_termination_listener_(std::move(load_termination_listener)),
      save_state_(NONE),
      page_load_state_(SUCCESS),
      network_bytes_(0LL),
      is_low_bar_met_(false),
      did_snapshot_on_last_retry_(false),
      weak_ptr_factory_(this) {
  DCHECK(offline_page_model_);
  DCHECK(browser_context_);
  if (load_termination_listener_)
    load_termination_listener_->set_offliner(this);

  for (int i = 0; i < ResourceDataType::RESOURCE_DATA_TYPE_COUNT; ++i) {
    stats_[i].requested = 0;
    stats_[i].completed = 0;
  }
}
