bool PasswordAutofillAgent::FillUserNameAndPassword(
    WebInputElement username_element,
    WebInputElement password_element,
    const PasswordFormFillData& fill_data,
    RendererSavePasswordProgressLogger* logger) {
  if (logger)
    logger->LogMessage(Logger::STRING_FILL_USERNAME_AND_PASSWORD_METHOD);

  bool is_single_username_fill = password_element.IsNull();
  WebInputElement main_element =
      is_single_username_fill ? username_element : password_element;

  if (IsInCrossOriginIframe(main_element)) {
    if (logger)
      logger->LogMessage(Logger::STRING_FAILED_TO_FILL_INTO_IFRAME);
    LogFirstFillingResult(fill_data, FillingResult::kBlockedByFrameHierarchy);
    return false;
  }

  if (!IsElementEditable(main_element)) {
    if (logger) {
      logger->LogMessage(
          Logger::STRING_FAILED_TO_FILL_NO_AUTOCOMPLETEABLE_ELEMENT);
    }
    LogFirstFillingResult(fill_data,
                          FillingResult::kPasswordElementIsNotAutocompleteable);
    return false;
  }

  base::string16 current_username;

  bool prefilled_placeholder_username = false;

  if (!username_element.IsNull()) {
    std::string possible_email_domain =
        GetRegistryControlledDomain(fill_data.origin);

    prefilled_placeholder_username =
        !username_element.Value().IsEmpty() &&
        (PossiblePrefilledUsernameValue(username_element.Value().Utf8(),
                                        possible_email_domain) ||
         fill_data.username_may_use_prefilled_placeholder);
    if (!username_element.Value().IsEmpty() &&
        !prefilled_placeholder_username) {
      current_username = username_element.Value().Utf16();
    } else if (IsElementEditable(username_element)) {
      current_username = fill_data.username_field.value;
    }
  }

  base::string16 username;
  base::string16 password;

  bool exact_username_match =
      username_element.IsNull() || IsElementEditable(username_element);

  FindMatchesByUsername(fill_data, current_username, exact_username_match,
                        logger, &username, &password);

  if (password.empty() && !is_single_username_fill) {
    if (!username_element.IsNull() && !username_element.Value().IsEmpty() &&
        !prefilled_placeholder_username) {
      LogPrefilledUsernameFillOutcome(
          PrefilledUsernameFillOutcome::kPrefilledUsernameNotOverridden);
      if (logger)
        logger->LogMessage(Logger::STRING_FAILED_TO_FILL_PREFILLED_USERNAME);
      LogFirstFillingResult(
          fill_data, FillingResult::kUsernamePrefilledWithIncompatibleValue);
      return false;
    }
    if (logger) {
      logger->LogMessage(
          Logger::STRING_FAILED_TO_FILL_FOUND_NO_PASSWORD_FOR_USERNAME);
    }
    LogFirstFillingResult(fill_data,
                          FillingResult::kFoundNoPasswordForUsername);
    return false;
  }

  if (password_generation_agent_ && !is_single_username_fill)
    password_generation_agent_->OnFieldAutofilled(password_element);

  if (!username_element.IsNull() && IsElementEditable(username_element)) {
    if (!username.empty() && (username_element.Value().IsEmpty() ||
                              prefilled_placeholder_username)) {
      AutofillField(username, username_element);
      if (prefilled_placeholder_username) {
        LogPrefilledUsernameFillOutcome(
            PrefilledUsernameFillOutcome::
                kPrefilledPlaceholderUsernameOverridden);
      }
    }
    username_element.SetAutofillState(WebAutofillState::kAutofilled);
    if (logger)
      logger->LogElementName(Logger::STRING_USERNAME_FILLED, username_element);
  }

  if (!is_single_username_fill) {
    AutofillField(password, password_element);
    if (logger)
      logger->LogElementName(Logger::STRING_PASSWORD_FILLED, password_element);
  }

  LogFirstFillingResult(fill_data, FillingResult::kSuccess);
  return true;
}
