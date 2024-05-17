void RenderFrameImpl::DidCreateScriptContext(v8::Local<v8::Context> context,
                                             int world_id) {
  if ((enabled_bindings_ & BINDINGS_POLICY_MOJO_WEB_UI) && IsMainFrame() &&
      world_id == ISOLATED_WORLD_ID_GLOBAL) {
    blink::WebContextFeatures::EnableMojoJS(context, true);
  }

  for (auto& observer : observers_)
    observer.DidCreateScriptContext(context, world_id);
}