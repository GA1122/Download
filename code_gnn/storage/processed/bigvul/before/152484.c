void RenderFrameImpl::PrepareFrameForCommit(
    const GURL& url,
    const CommitNavigationParams& commit_params) {
  browser_side_navigation_pending_ = false;
  browser_side_navigation_pending_url_ = GURL();
  sync_navigation_callback_.Cancel();
  mhtml_body_loader_client_.reset();

  GetContentClient()->SetActiveURL(
      url, frame_->Top()->GetSecurityOrigin().ToString().Utf8());

  RenderFrameImpl::PrepareRenderViewForNavigation(url, commit_params);
}
