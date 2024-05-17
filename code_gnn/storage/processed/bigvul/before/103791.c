ui::TextInputType RenderView::GetTextInputType() {
  if (pepper_delegate_.IsPluginFocused()) {
    return ui::TEXT_INPUT_TYPE_TEXT;
  }
  return RenderWidget::GetTextInputType();
}
