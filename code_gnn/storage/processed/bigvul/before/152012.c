void RenderFrameHostImpl::OnAccessibilityEvents(
    const AccessibilityHostMsg_EventBundleParams& bundle,
    int reset_token,
    int ack_token) {
  if (accessibility_reset_token_ != reset_token) {
    Send(new AccessibilityMsg_EventBundle_ACK(routing_id_, ack_token));
    return;
  }
  accessibility_reset_token_ = 0;

  RenderWidgetHostViewBase* view = GetViewForAccessibility();
  ui::AXMode accessibility_mode = delegate_->GetAccessibilityMode();
  if (!accessibility_mode.is_mode_off() && view && is_active()) {
    if (accessibility_mode.has_mode(ui::AXMode::kNativeAPIs))
      GetOrCreateBrowserAccessibilityManager();

    AXEventNotificationDetails details;
    details.ax_tree_id = GetAXTreeID();
    details.events = bundle.events;

    details.updates.resize(bundle.updates.size());
    for (size_t i = 0; i < bundle.updates.size(); ++i) {
      const AXContentTreeUpdate& src_update = bundle.updates[i];
      ui::AXTreeUpdate* dst_update = &details.updates[i];
      if (src_update.has_tree_data) {
        dst_update->has_tree_data = true;
        ax_content_tree_data_ = src_update.tree_data;
        AXContentTreeDataToAXTreeData(&dst_update->tree_data);
      }
      dst_update->root_id = src_update.root_id;
      dst_update->node_id_to_clear = src_update.node_id_to_clear;
      dst_update->event_from = src_update.event_from;
      dst_update->nodes.resize(src_update.nodes.size());
      for (size_t j = 0; j < src_update.nodes.size(); ++j) {
        AXContentNodeDataToAXNodeData(src_update.nodes[j],
                                      &dst_update->nodes[j]);
      }
    }

    if (accessibility_mode.has_mode(ui::AXMode::kNativeAPIs)) {
      if (browser_accessibility_manager_)
        browser_accessibility_manager_->OnAccessibilityEvents(details);
    }

    delegate_->AccessibilityEventReceived(details);

    if (!accessibility_testing_callback_.is_null()) {
      if (details.events.empty()) {
        accessibility_testing_callback_.Run(this, ax::mojom::Event::kNone, 0);
      } else {
        for (size_t i = 0; i < details.events.size(); i++) {
          if (static_cast<int>(details.events[i].event_type) < 0)
            continue;

          accessibility_testing_callback_.Run(
              this, details.events[i].event_type, details.events[i].id);
        }
      }
    }
  }

  Send(new AccessibilityMsg_EventBundle_ACK(routing_id_, ack_token));
}
