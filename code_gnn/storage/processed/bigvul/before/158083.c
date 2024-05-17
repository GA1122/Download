void LocalFrameClientImpl::DispatchWillSubmitForm(HTMLFormElement* form) {
  if (web_frame_->Client())
    web_frame_->Client()->WillSubmitForm(WebFormElement(form));
}
