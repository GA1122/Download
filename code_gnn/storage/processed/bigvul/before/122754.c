void BrowserPluginGuest::HandleInputEventAck(RenderViewHost* render_view_host,
                                             bool handled) {
  RenderViewHostImpl* guest_rvh =
      static_cast<RenderViewHostImpl*>(render_view_host);
  guest_rvh->StopHangMonitorTimeout();
  DCHECK(pending_input_event_reply_.get());
  IPC::Message* reply_message = pending_input_event_reply_.release();
  BrowserPluginHostMsg_HandleInputEvent::WriteReplyParams(reply_message,
                                                          handled);
  SendMessageToEmbedder(reply_message);
}
