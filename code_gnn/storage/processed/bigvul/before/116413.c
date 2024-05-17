void ChromeContentRendererClient::WillReleaseScriptContext(
    WebFrame* frame, v8::Handle<v8::Context> context, int world_id) {
  extension_dispatcher_->WillReleaseScriptContext(frame, context, world_id);
}
