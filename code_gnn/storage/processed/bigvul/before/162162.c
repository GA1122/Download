void RenderProcessHostImpl::WidgetRestored() {
  visible_widgets_++;
  UpdateProcessPriority();
}
