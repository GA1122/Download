void LayerTreeHostImpl::MarkUIResourceNotEvicted(UIResourceId uid) {
  std::set<UIResourceId>::iterator found_in_evicted =
      evicted_ui_resources_.find(uid);
  if (found_in_evicted == evicted_ui_resources_.end())
    return;
  evicted_ui_resources_.erase(found_in_evicted);
  if (evicted_ui_resources_.empty())
    client_->OnCanDrawStateChanged(CanDraw());
}
