void RenderFrameImpl::PepperInstanceDeleted(
    PepperPluginInstanceImpl* instance) {
  active_pepper_instances_.erase(instance);

  if (pepper_last_mouse_event_target_ == instance)
    pepper_last_mouse_event_target_ = nullptr;
  if (focused_pepper_plugin_ == instance)
    PepperFocusChanged(instance, false);

  RenderFrameImpl* const render_frame = instance->render_frame();
  if (render_frame) {
    render_frame->Send(
        new FrameHostMsg_PepperInstanceDeleted(
            render_frame->GetRoutingID(),
            instance->pp_instance()));
  }
}
