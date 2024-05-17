void WebFrameLoaderClient::dispatchWillSubmitForm(FramePolicyFunction function,
    PassRefPtr<FormState> form_ref) {
  if (webframe_->client()) {
    webframe_->client()->willSubmitForm(
        webframe_, webkit_glue::HTMLFormElementToWebForm(form_ref->form()));
  }
  (webframe_->frame()->loader()->*function)(PolicyUse);
}
