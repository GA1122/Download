void RenderViewImpl::willReleaseScriptContext(WebFrame* frame,
                                              v8::Handle<v8::Context> context,
                                              int world_id) {
  content::GetContentClient()->renderer()->WillReleaseScriptContext(
      frame, context, world_id);
}
