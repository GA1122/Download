void RenderFrameImpl::WillSendSubmitEvent(const blink::WebFormElement& form) {
  for (auto& observer : observers_)
    observer.WillSendSubmitEvent(form);
}
