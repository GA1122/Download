void LocalFrameClientImpl::WillReleaseScriptContext(
    v8::Local<v8::Context> context,
    int world_id) {
  if (web_frame_->Client()) {
    web_frame_->Client()->WillReleaseScriptContext(context, world_id);
  }
}
