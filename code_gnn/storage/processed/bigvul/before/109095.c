void RenderViewImpl::didCreateScriptContext(WebFrame* frame,
                                            v8::Handle<v8::Context> context,
                                            int world_id) {
  content::GetContentClient()->renderer()->DidCreateScriptContext(
      frame, context, -1, world_id);
}
