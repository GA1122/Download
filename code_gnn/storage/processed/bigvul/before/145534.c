void ResourceDispatcherHostImpl::UpdateLoadInfoOnUIThread(
    scoped_ptr<LoadInfoMap> info_map) {
  tracked_objects::ScopedTracker tracking_profile(
      FROM_HERE_WITH_EXPLICIT_FUNCTION(
          "466285 ResourceDispatcherHostImpl::UpdateLoadInfoOnUIThread"));
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  for (const auto& load_info : *info_map) {
    RenderViewHostImpl* view = RenderViewHostImpl::FromID(
        load_info.first.child_id, load_info.first.route_id);
    if (view) {
      view->LoadStateChanged(load_info.second.url, load_info.second.load_state,
                             load_info.second.upload_position,
                             load_info.second.upload_size);
    }
  }
}
