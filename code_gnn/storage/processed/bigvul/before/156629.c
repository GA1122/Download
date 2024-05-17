bool NavigatorImpl::StartHistoryNavigationInNewSubframe(
    RenderFrameHostImpl* render_frame_host,
    const GURL& default_url) {
  return controller_->StartHistoryNavigationInNewSubframe(render_frame_host,
                                                          default_url);
}
