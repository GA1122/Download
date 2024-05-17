void RenderFrameHostImpl::OnAccessibilityChildFrameHitTestResult(
    const gfx::Point& point,
    int hit_obj_id,
    ui::AXEvent event_to_fire) {
  if (browser_accessibility_manager_) {
    browser_accessibility_manager_->OnChildFrameHitTestResult(point, hit_obj_id,
                                                              event_to_fire);
  }
}
