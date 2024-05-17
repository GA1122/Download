void RenderFrameImpl::WillSendSubmitEvent(const blink::WebFormElement& form) {
  SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.WillSendSubmitEvent");
  for (auto& observer : observers_)
    observer.WillSendSubmitEvent(form);
}
