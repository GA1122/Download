void RenderFrameHostImpl::UpdateAXTreeData() {
  AccessibilityMode accessibility_mode = delegate_->GetAccessibilityMode();
  if (accessibility_mode.is_mode_off() || !is_active()) {
    return;
  }

  std::vector<AXEventNotificationDetails> details;
  details.reserve(1U);
  AXEventNotificationDetails detail;
  detail.ax_tree_id = GetAXTreeID();
  detail.update.has_tree_data = true;
  AXContentTreeDataToAXTreeData(&detail.update.tree_data);
  details.push_back(detail);

  if (browser_accessibility_manager_)
    browser_accessibility_manager_->OnAccessibilityEvents(details);

  delegate_->AccessibilityEventReceived(details);
}
