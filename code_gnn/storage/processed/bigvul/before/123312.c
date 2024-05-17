void RenderWidgetHostViewGtk::MarkCachedWidgetCenterStale() {
  widget_center_valid_ = false;
  mouse_has_been_warped_to_new_center_ = false;
}
