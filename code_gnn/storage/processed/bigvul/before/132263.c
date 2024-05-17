void RenderFrameImpl::PrepareRenderViewForNavigation(
    const GURL& url,
    const RequestNavigationParams& request_params,
    bool* is_reload,
    WebURLRequest::CachePolicy* cache_policy) {
  DCHECK(render_view_->webview());

  MaybeHandleDebugURL(url);

  FOR_EACH_OBSERVER(
      RenderViewObserver, render_view_->observers_, Navigate(url));

  render_view_->history_list_offset_ =
      request_params.current_history_list_offset;
  render_view_->history_list_length_ =
      request_params.current_history_list_length;
  if (request_params.should_clear_history_list) {
    CHECK_EQ(-1, render_view_->history_list_offset_);
    CHECK_EQ(0, render_view_->history_list_length_);
  }

  if (!is_swapped_out_ || frame_->parent())
    return;

  render_view_->webview()->setVisibilityState(
      render_view_->visibilityState(), false);

  *is_reload = false;
  *cache_policy = WebURLRequest::ReloadIgnoringCacheData;

  RenderThreadImpl::NotifyTimezoneChange();

  render_view_->SetSwappedOut(false);
  is_swapped_out_ = false;
  return;
}
