void RenderWidget::SetHidden(bool hidden) {
  if (is_hidden_ == hidden)
    return;

  is_hidden_ = hidden;
  if (is_hidden_)
    render_thread_->WidgetHidden();
  else
    render_thread_->WidgetRestored();
}
