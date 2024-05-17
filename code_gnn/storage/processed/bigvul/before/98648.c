void RenderWidget::OnSetFocus(bool enable) {
  has_focus_ = enable;
  if (webwidget_)
    webwidget_->setFocus(enable);
  if (enable) {
    ime_control_updated_ = true;
    ime_control_new_state_ = true;
  }
}
