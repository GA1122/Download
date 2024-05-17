void RenderFrameImpl::didChangeManifest(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);

  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, DidChangeManifest());
}
