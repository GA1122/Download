void RenderViewImpl::didChangeLoadProgress(WebFrame* frame,
                                           double load_progress) {
  if (load_progress_tracker_ != NULL)
    load_progress_tracker_->DidChangeLoadProgress(frame, load_progress);
}
