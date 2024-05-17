void RenderWidgetHostViewAura::ExtendSelectionAndDelete(
    size_t before, size_t after) {
  if (host_)
    host_->ExtendSelectionAndDelete(before, after);
}
