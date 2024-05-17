bool FillUserNameAndPassword(
    blink::WebInputElement* username_element,
    blink::WebInputElement* password_element,
    const PasswordFormFillData& fill_data,
    bool exact_username_match,
    bool set_selection,
    std::map<const blink::WebInputElement, blink::WebString>*
        nonscript_modified_values,
    base::Callback<void(blink::WebInputElement*)> registration_callback,
    RendererSavePasswordProgressLogger* logger) {
  if (logger)
    logger->LogMessage(Logger::STRING_FILL_USERNAME_AND_PASSWORD_METHOD);

  if (!IsElementAutocompletable(*password_element))
    return false;

  base::string16 current_username;
  if (!username_element->isNull()) {
    current_username = username_element->value();
  }

  base::string16 username;
  base::string16 password;

  if (DoUsernamesMatch(fill_data.username_field.value, current_username,
                       exact_username_match)) {
    username = fill_data.username_field.value;
    password = fill_data.password_field.value;
    if (logger)
      logger->LogMessage(Logger::STRING_USERNAMES_MATCH);
  } else {
    for (const auto& it : fill_data.additional_logins) {
      if (DoUsernamesMatch(it.first, current_username, exact_username_match)) {
        username = it.first;
        password = it.second.password;
        break;
      }
    }
    if (logger) {
      logger->LogBoolean(Logger::STRING_MATCH_IN_ADDITIONAL,
                         !(username.empty() && password.empty()));
    }

    if (username.empty() && password.empty()) {
      for (const auto& it : fill_data.other_possible_usernames) {
        for (size_t i = 0; i < it.second.size(); ++i) {
          if (DoUsernamesMatch(
                  it.second[i], current_username, exact_username_match)) {
            username = it.second[i];
            password = it.first.password;
            break;
          }
        }
        if (!username.empty() && !password.empty())
          break;
      }
    }
  }
  if (password.empty())
    return false;


  if (!username_element->isNull() &&
      IsElementAutocompletable(*username_element)) {
    username_element->setValue(username, true);
    (*nonscript_modified_values)[*username_element] = username;
    username_element->setAutofilled(true);
    if (logger)
      logger->LogElementName(Logger::STRING_USERNAME_FILLED, *username_element);
    if (set_selection) {
      form_util::PreviewSuggestion(username, current_username,
                                   username_element);
    }
  } else if (current_username != username) {
    return false;
  }

  password_element->setSuggestedValue(password);
  (*nonscript_modified_values)[*password_element] = password;
  registration_callback.Run(password_element);

  password_element->setAutofilled(true);
  if (logger)
    logger->LogElementName(Logger::STRING_PASSWORD_FILLED, *password_element);
  return true;
}
