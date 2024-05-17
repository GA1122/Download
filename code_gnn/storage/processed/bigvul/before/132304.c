void RenderFrameImpl::didCreateNewDocument(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);

  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, DidCreateNewDocument());
  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidCreateNewDocument(frame));
}
