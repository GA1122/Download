void RenderViewImpl::willSendSubmitEvent(WebKit::WebFrame* frame,
    const WebKit::WebFormElement& form) {
  DocumentState* document_state =
      DocumentState::FromDataSource(frame->dataSource());
  document_state->set_password_form_data(CreatePasswordForm(form));
}
