bool RenderViewImpl::HasIMETextFocus() {
  return GetTextInputType() != ui::TEXT_INPUT_TYPE_NONE;
}
