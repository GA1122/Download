void LocalFrameClientImpl::DispatchWillSendSubmitEvent(HTMLFormElement* form) {
  if (web_frame_->Client())
    web_frame_->Client()->WillSendSubmitEvent(WebFormElement(form));
}
