void WebContentsImpl::SendChangeLoadProgress() {
  loading_last_progress_update_ = base::TimeTicks::Now();
  if (delegate_)
    delegate_->LoadProgressChanged(this, frame_tree_.load_progress());
}
