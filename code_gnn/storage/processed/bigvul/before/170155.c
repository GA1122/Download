void PasswordAutofillAgent::OnSamePageNavigationCompleted() {
  if (!ProvisionallySavedPasswordIsValid())
    return;

  blink::WebFrame* frame = render_frame()->GetWebFrame();
  if (form_util::IsFormVisible(frame, provisionally_saved_form_->action,
                               provisionally_saved_form_->origin,
                               provisionally_saved_form_->form_data) ||
      IsUnownedPasswordFormVisible(frame, provisionally_saved_form_->action,
                                   provisionally_saved_form_->origin,
                                   provisionally_saved_form_->form_data,
                                   form_predictions_)) {
    return;
  }

  Send(new AutofillHostMsg_InPageNavigation(routing_id(),
                                            *provisionally_saved_form_));
  provisionally_saved_form_.reset();
}
