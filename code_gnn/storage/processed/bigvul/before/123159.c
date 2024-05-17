void RenderWidgetHostViewAura::Destroy() {
  in_shutdown_ = true;
  delete window_;
}
