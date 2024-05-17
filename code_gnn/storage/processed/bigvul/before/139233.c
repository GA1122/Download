void RenderProcessHostImpl::WidgetRestored() {
  visible_widgets_++;
  UpdateProcessPriority();
  DCHECK(!is_process_backgrounded_);
}
