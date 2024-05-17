void PasswordAutofillAgent::WillSubmitForm(const blink::WebFormElement& form) {
  std::unique_ptr<RendererSavePasswordProgressLogger> logger;
  if (logging_state_active_) {
    logger.reset(new RendererSavePasswordProgressLogger(this, routing_id()));
    logger->LogMessage(Logger::STRING_WILL_SUBMIT_FORM_METHOD);
    LogHTMLForm(logger.get(), Logger::STRING_HTML_FORM_FOR_SUBMIT, form);
  }

  std::unique_ptr<PasswordForm> submitted_form = CreatePasswordFormFromWebForm(
      form, &nonscript_modified_values_, &form_predictions_);

  if (submitted_form) {
    if (logger) {
      logger->LogPasswordForm(Logger::STRING_CREATED_PASSWORD_FORM,
                              *submitted_form);
    }
    if (provisionally_saved_form_ &&
        submitted_form->action == provisionally_saved_form_->action) {
      if (logger)
        logger->LogMessage(Logger::STRING_SUBMITTED_PASSWORD_REPLACED);
      submitted_form->password_value =
          provisionally_saved_form_->password_value;
      submitted_form->new_password_value =
          provisionally_saved_form_->new_password_value;
      submitted_form->username_value =
          provisionally_saved_form_->username_value;
    }

    Send(new AutofillHostMsg_PasswordFormSubmitted(routing_id(),
                                                   *submitted_form));
    provisionally_saved_form_.reset();
  } else if (logger) {
    logger->LogMessage(Logger::STRING_FORM_IS_NOT_PASSWORD);
  }
}
