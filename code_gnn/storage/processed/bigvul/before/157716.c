void WebContentsImpl::AdjustPreviewsStateForNavigation(
    PreviewsState* previews_state) {
  if (delegate_)
    delegate_->AdjustPreviewsStateForNavigation(this, previews_state);
}
