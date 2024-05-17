void RenderFrameHostImpl::UpdateAXTreeData() {
  ui::AXMode accessibility_mode = delegate_->GetAccessibilityMode();
  if (accessibility_mode.is_mode_off() || !is_active()) {
    return;
  }

  AXEventNotificationDetails detail;
  detail.ax_tree_id = GetAXTreeID();
  detail.updates.resize(1);
  detail.updates[0].has_tree_data = true;
  AXContentTreeDataToAXTreeData(&detail.updates[0].tree_data);

  if (browser_accessibility_manager_)
    browser_accessibility_manager_->OnAccessibilityEvents(detail);

  delegate_->AccessibilityEventReceived(detail);
}
