void RenderFrameImpl::PepperDidReceiveMouseEvent(
    PepperPluginInstanceImpl* instance) {
  set_pepper_last_mouse_event_target(instance);
}
