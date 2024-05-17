void RenderProcessHostImpl::WidgetHidden() {
  if (visible_widgets_ == 0)
    return;

  --visible_widgets_;
  if (visible_widgets_ == 0) {
    UpdateProcessPriority();
  }
}
