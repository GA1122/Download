void RenderFrameImpl::PepperTextInputTypeChanged(
    PepperPluginInstanceImpl* instance) {
  if (instance != focused_pepper_plugin_)
    return;

  GetRenderWidget()->UpdateTextInputState();

  FocusedNodeChangedForAccessibility(WebNode());
}
