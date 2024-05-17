void RenderWidgetHostViewAura::Shutdown() {
  if (!in_shutdown_) {
    in_shutdown_ = true;
    host_->ShutdownAndDestroyWidget(true);
  }
}
