blink::WebPresentationClient* RenderFrameImpl::presentationClient() {
  if (!presentation_dispatcher_)
    presentation_dispatcher_ = new PresentationDispatcher(this);
  return presentation_dispatcher_;
}
