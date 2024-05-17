void BrowserPluginGuest::HandleInputEvent(RenderViewHost* render_view_host,
                                          const gfx::Rect& guest_window_rect,
                                          const gfx::Rect& guest_screen_rect,
                                          const WebKit::WebInputEvent& event,
                                          IPC::Message* reply_message) {
  DCHECK(!pending_input_event_reply_.get());
  guest_window_rect_ = guest_window_rect;
  guest_screen_rect_ = guest_screen_rect;
  RenderViewHostImpl* guest_rvh = static_cast<RenderViewHostImpl*>(
      web_contents()->GetRenderViewHost());
  IPC::Message* message = new ViewMsg_HandleInputEvent(routing_id());

  scoped_array<char> input_buffer(new char[event.size]);
  memcpy(input_buffer.get(), &event, event.size);
  WebKit::WebInputEvent* input_event =
      reinterpret_cast<WebKit::WebInputEvent*>(input_buffer.get());
  if (event.type == WebKit::WebInputEvent::KeyDown)
    input_event->type = WebKit::WebInputEvent::RawKeyDown;

  message->WriteData(input_buffer.get(), event.size);
  if (input_event->type == WebKit::WebInputEvent::RawKeyDown)
    message->WriteBool(false);
  if (!Send(message)) {
    BrowserPluginHostMsg_HandleInputEvent::WriteReplyParams(
        reply_message, false  );
    SendMessageToEmbedder(reply_message);
    return;
  }

  pending_input_event_reply_.reset(reply_message);
  guest_rvh->StartHangMonitorTimeout(guest_hang_timeout_);
}
