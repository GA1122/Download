void RenderFrameImpl::didCreateScriptContext(blink::WebLocalFrame* frame,
                                             v8::Local<v8::Context> context,
                                             int extension_group,
                                             int world_id) {
  DCHECK(!frame_ || frame_ == frame);

  FOR_EACH_OBSERVER(RenderFrameObserver, observers_,
                    DidCreateScriptContext(context, extension_group, world_id));
}
