void RenderFrameImpl::OnDroppedNavigation() {
  browser_side_navigation_pending_ = false;
  browser_side_navigation_pending_url_ = GURL();
  frame_->ClientDroppedNavigation();
}
