void PasswordAutofillAgent::OnWillSubmitForm(const WebFormElement& form) {
  std::unique_ptr<RendererSavePasswordProgressLogger> logger;
  if (logging_state_active_) {
    logger.reset(new RendererSavePasswordProgressLogger(
        GetPasswordManagerDriver().get()));
    logger->LogMessage(Logger::STRING_WILL_SUBMIT_FORM_METHOD);
    LogHTMLForm(logger.get(), Logger::STRING_HTML_FORM_FOR_SUBMIT, form);
  }

  std::unique_ptr<PasswordForm> submitted_form =
      GetPasswordFormFromWebForm(form);

  if (submitted_form) {
    if (logger) {
      logger->LogPasswordForm(Logger::STRING_CREATED_PASSWORD_FORM,
                              *submitted_form);
    }
    submitted_form->submission_event =
        SubmissionIndicatorEvent::HTML_FORM_SUBMISSION;
    submitted_form->form_data.submission_event =
        SubmissionIndicatorEvent::HTML_FORM_SUBMISSION;

    if (FrameCanAccessPasswordManager()) {
      GetPasswordManagerDriver()->PasswordFormSubmitted(*submitted_form);
    } else {
      if (logger)
        logger->LogMessage(Logger::STRING_SECURITY_ORIGIN_FAILURE);
    }
    browser_has_form_to_process_ = false;
  } else if (logger) {
    logger->LogMessage(Logger::STRING_FORM_IS_NOT_PASSWORD);
  }
}
