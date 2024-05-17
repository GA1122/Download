void RenderWidgetHostImpl::AcknowledgeSwapBuffersToRenderer() {
  if (!is_threaded_compositing_enabled_)
    Send(new ViewMsg_SwapBuffers_ACK(routing_id_));
}
