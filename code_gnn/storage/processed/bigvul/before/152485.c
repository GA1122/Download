void RenderFrameImpl::PrepareRenderViewForNavigation(
    const GURL& url,
    const CommitNavigationParams& commit_params) {
  DCHECK(render_view_->webview());

  if (is_main_frame_) {
    for (auto& observer : render_view_->observers_)
      observer.Navigate(url);
  }

  render_view_->history_list_offset_ =
      commit_params.current_history_list_offset;
  render_view_->history_list_length_ =
      commit_params.current_history_list_length;
}
