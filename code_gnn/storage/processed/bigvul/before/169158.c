void RenderFrameHostImpl::OnAccessibilityEvents(
    const std::vector<AccessibilityHostMsg_EventParams>& params,
    int reset_token, int ack_token) {
  if (accessibility_reset_token_ != reset_token) {
    Send(new AccessibilityMsg_Events_ACK(routing_id_, ack_token));
    return;
  }
  accessibility_reset_token_ = 0;

  RenderWidgetHostViewBase* view = GetViewForAccessibility();
  ui::AXMode accessibility_mode = delegate_->GetAccessibilityMode();
  if (!accessibility_mode.is_mode_off() && view && is_active()) {
    if (accessibility_mode.has_mode(ui::AXMode::kNativeAPIs))
      GetOrCreateBrowserAccessibilityManager();

    std::vector<AXEventNotificationDetails> details;
    details.reserve(params.size());
    for (size_t i = 0; i < params.size(); ++i) {
      const AccessibilityHostMsg_EventParams& param = params[i];
      AXEventNotificationDetails detail;
      detail.event_type = param.event_type;
      detail.id = param.id;
      detail.ax_tree_id = GetAXTreeID();
      detail.event_from = param.event_from;
      detail.action_request_id = param.action_request_id;
      if (param.update.has_tree_data) {
        detail.update.has_tree_data = true;
        ax_content_tree_data_ = param.update.tree_data;
        AXContentTreeDataToAXTreeData(&detail.update.tree_data);
      }
      detail.update.root_id = param.update.root_id;
      detail.update.node_id_to_clear = param.update.node_id_to_clear;
      detail.update.nodes.resize(param.update.nodes.size());
      for (size_t j = 0; j < param.update.nodes.size(); ++j) {
        AXContentNodeDataToAXNodeData(param.update.nodes[j],
                                      &detail.update.nodes[j]);
      }
      details.push_back(detail);
    }

    if (accessibility_mode.has_mode(ui::AXMode::kNativeAPIs)) {
      if (browser_accessibility_manager_)
        browser_accessibility_manager_->OnAccessibilityEvents(details);
    }

    delegate_->AccessibilityEventReceived(details);

    if (!accessibility_testing_callback_.is_null()) {
      for (size_t i = 0; i < details.size(); i++) {
        const AXEventNotificationDetails& detail = details[i];
        if (static_cast<int>(detail.event_type) < 0)
          continue;

        if (!ax_tree_for_testing_) {
          if (browser_accessibility_manager_) {
            ax_tree_for_testing_.reset(new ui::AXTree(
                browser_accessibility_manager_->SnapshotAXTreeForTesting()));
          } else {
            ax_tree_for_testing_.reset(new ui::AXTree());
            CHECK(ax_tree_for_testing_->Unserialize(detail.update))
                << ax_tree_for_testing_->error();
          }
        } else {
          CHECK(ax_tree_for_testing_->Unserialize(detail.update))
              << ax_tree_for_testing_->error();
        }
        accessibility_testing_callback_.Run(this, detail.event_type, detail.id);
      }
    }
  }

  Send(new AccessibilityMsg_Events_ACK(routing_id_, ack_token));
}
