void RenderFrameImpl::PepperStopsPlayback(PepperPluginInstanceImpl* instance) {
  RenderFrameImpl* const render_frame = instance->render_frame();
  if (render_frame) {
    render_frame->Send(
        new FrameHostMsg_PepperStopsPlayback(
            render_frame->GetRoutingID(),
            instance->pp_instance()));
  }
}
