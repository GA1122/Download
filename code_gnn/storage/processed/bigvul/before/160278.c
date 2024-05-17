void MimeHandlerViewContainer::OnMimeHandlerViewGuestOnLoadCompleted(
    int  ) {
  if (!render_frame())
    return;

  guest_loaded_ = true;
  if (pending_messages_.empty())
    return;

  blink::WebLocalFrame* frame = render_frame()->GetWebFrame();
  if (!frame)
    return;

  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(frame->MainWorldScriptContext());
  for (const auto& pending_message : pending_messages_)
    PostJavaScriptMessage(isolate,
                          v8::Local<v8::Value>::New(isolate, pending_message));

  pending_messages_.clear();
}
