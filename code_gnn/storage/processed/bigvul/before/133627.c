void SendToAllFramesInternal(IPC::Message* message, RenderFrameHost* rfh) {
  IPC::Message* message_copy = new IPC::Message(*message);
  message_copy->set_routing_id(rfh->GetRoutingID());
  rfh->Send(message_copy);
}
