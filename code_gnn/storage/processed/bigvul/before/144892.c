void RenderWidgetHostViewAura::Destroy() {
  in_shutdown_ = true;
  if (window_)
    delete window_;
  else
    delete this;
}
