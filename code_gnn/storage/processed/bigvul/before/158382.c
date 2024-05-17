bool RenderWidgetHostImpl::SynchronizeVisualProperties(
    bool scroll_focused_node_into_view) {
  if (visual_properties_ack_pending_ || !process_->IsInitializedAndNotDead() ||
      !view_ || !view_->HasSize() || !renderer_initialized_ || !delegate_) {
    return false;
  }

  std::unique_ptr<VisualProperties> visual_properties(new VisualProperties);
  bool needs_ack = false;
  if (!GetVisualProperties(visual_properties.get(), &needs_ack))
    return false;
  visual_properties->scroll_focused_node_into_view =
      scroll_focused_node_into_view;

  ScreenInfo screen_info = visual_properties->screen_info;
  bool width_changed =
      !old_visual_properties_ || old_visual_properties_->new_size.width() !=
                                     visual_properties->new_size.width();
  bool sent_visual_properties = false;
  if (Send(new ViewMsg_SynchronizeVisualProperties(routing_id_,
                                                   *visual_properties))) {
    visual_properties_ack_pending_ = needs_ack;
    old_visual_properties_.swap(visual_properties);
    sent_visual_properties = true;
  }

  if (delegate_)
    delegate_->RenderWidgetWasResized(this, screen_info, width_changed);

  return sent_visual_properties;
}
