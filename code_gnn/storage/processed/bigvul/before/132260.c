void RenderFrameImpl::PepperTextInputTypeChanged(
    PepperPluginInstanceImpl* instance) {
  if (instance != render_view_->focused_pepper_plugin())
    return;

  GetRenderWidget()->UpdateTextInputState(
      RenderWidget::NO_SHOW_IME, RenderWidget::FROM_NON_IME);

  FocusedNodeChangedForAccessibility(WebNode());
}
