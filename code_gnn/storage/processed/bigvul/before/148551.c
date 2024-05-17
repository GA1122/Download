int WebContentsImpl::SendToAllFrames(IPC::Message* message) {
  int number_of_messages = 0;
  for (RenderFrameHost* rfh : GetAllFrames()) {
    if (!rfh->IsRenderFrameLive())
      continue;

    ++number_of_messages;
    IPC::Message* message_copy = new IPC::Message(*message);
    message_copy->set_routing_id(rfh->GetRoutingID());
    rfh->Send(message_copy);
  }
  delete message;
  return number_of_messages;
}
