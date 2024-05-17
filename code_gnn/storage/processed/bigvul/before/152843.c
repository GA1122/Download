void MetricsWebContentsObserver::OnVisibilityChanged(
    content::Visibility visibility) {
  if (web_contents_will_soon_be_destroyed_)
    return;

  bool was_in_foreground = in_foreground_;
  in_foreground_ = visibility == content::Visibility::VISIBLE;
  if (in_foreground_ == was_in_foreground)
    return;

  if (in_foreground_) {
    if (committed_load_)
      committed_load_->WebContentsShown();
    for (const auto& kv : provisional_loads_) {
      kv.second->WebContentsShown();
    }
  } else {
    if (committed_load_)
      committed_load_->WebContentsHidden();
    for (const auto& kv : provisional_loads_) {
      kv.second->WebContentsHidden();
    }
  }
}
