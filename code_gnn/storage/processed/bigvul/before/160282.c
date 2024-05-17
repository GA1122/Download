void MimeHandlerViewContainer::PostJavaScriptMessage(
    v8::Isolate* isolate,
    v8::Local<v8::Value> message) {
  if (!guest_loaded_) {
    pending_messages_.push_back(v8::Global<v8::Value>(isolate, message));
    return;
  }

  content::RenderView* guest_proxy_render_view =
      content::RenderView::FromRoutingID(guest_proxy_routing_id_);
  if (!guest_proxy_render_view)
    return;
  blink::WebFrame* guest_proxy_frame =
      guest_proxy_render_view->GetWebView()->MainFrame();
  if (!guest_proxy_frame)
    return;

  v8::Context::Scope context_scope(
      render_frame()->GetWebFrame()->MainWorldScriptContext());

  v8::Local<v8::Object> guest_proxy_window = guest_proxy_frame->GlobalProxy();
  gin::Dictionary window_object(isolate, guest_proxy_window);
  v8::Local<v8::Function> post_message;
  if (!window_object.Get(std::string(kPostMessageName), &post_message))
    return;

  v8::Local<v8::Value> args[] = {
      message,
      gin::StringToV8(isolate, "*")};
  render_frame()->GetWebFrame()->CallFunctionEvenIfScriptDisabled(
      post_message.As<v8::Function>(), guest_proxy_window, arraysize(args),
      args);
}
