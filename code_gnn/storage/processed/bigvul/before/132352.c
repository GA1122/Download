void RenderFrameImpl::willSendSubmitEvent(blink::WebLocalFrame* frame,
                                          const blink::WebFormElement& form) {
  DCHECK(!frame_ || frame_ == frame);

  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, WillSendSubmitEvent(form));
}
