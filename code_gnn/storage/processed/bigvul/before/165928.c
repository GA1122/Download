void RenderFrameImpl::PrepareFrameForCommit(
    const GURL& url,
    const RequestNavigationParams& request_params) {
  browser_side_navigation_pending_ = false;
  browser_side_navigation_pending_url_ = GURL();
  sync_navigation_callback_.Cancel();

  GetContentClient()->SetActiveURL(
      url, frame_->Top()->GetSecurityOrigin().ToString().Utf8());

  RenderFrameImpl::PrepareRenderViewForNavigation(url, request_params);
}
