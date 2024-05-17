void RenderWidgetHostImpl::ResetSentVisualProperties() {
  visual_properties_ack_pending_ = false;
  old_visual_properties_.reset();
}
