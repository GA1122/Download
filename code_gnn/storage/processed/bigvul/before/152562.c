void RenderFrameImpl::WillReleaseScriptContext(v8::Local<v8::Context> context,
                                               int world_id) {
  for (auto& observer : observers_)
    observer.WillReleaseScriptContext(context, world_id);
}
