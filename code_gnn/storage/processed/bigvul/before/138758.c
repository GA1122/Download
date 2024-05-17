void RenderFrameHostImpl::OnNavigationHandledByEmbedder() {
  if (navigation_handle_)
    navigation_handle_->set_net_error_code(net::ERR_ABORTED);

  OnDidStopLoading();
}
