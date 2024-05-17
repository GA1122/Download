void WebContentsImpl::ResetLoadProgressState() {
  frame_tree_.ResetLoadProgress();
  loading_weak_factory_.InvalidateWeakPtrs();
  loading_last_progress_update_ = base::TimeTicks();
}
