void PasswordAutofillAgent::OnFillPasswordForm(
    int key,
    const PasswordFormFillData& form_data) {
  std::unique_ptr<RendererSavePasswordProgressLogger> logger;
  if (logging_state_active_) {
    logger.reset(new RendererSavePasswordProgressLogger(this, routing_id()));
    logger->LogMessage(Logger::STRING_ON_FILL_PASSWORD_FORM_METHOD);
  }

  bool ambiguous_or_empty_names =
      DoesFormContainAmbiguousOrEmptyNames(form_data);
  FormElementsList forms;
  FindFormElements(render_frame(), form_data, ambiguous_or_empty_names, &forms);
  if (logger) {
    logger->LogBoolean(Logger::STRING_AMBIGUOUS_OR_EMPTY_NAMES,
                       ambiguous_or_empty_names);
    logger->LogNumber(Logger::STRING_NUMBER_OF_POTENTIAL_FORMS_TO_FILL,
                      forms.size());
    logger->LogBoolean(Logger::STRING_FORM_DATA_WAIT,
                       form_data.wait_for_username);
  }
  for (const auto& form : forms) {
    base::string16 username_field_name;
    base::string16 password_field_name =
        FieldName(form_data.password_field, ambiguous_or_empty_names);
    bool form_contains_fillable_username_field =
        FillDataContainsFillableUsername(form_data);
    if (form_contains_fillable_username_field) {
      username_field_name =
          FieldName(form_data.username_field, ambiguous_or_empty_names);
    }
    if (logger) {
      logger->LogBoolean(Logger::STRING_CONTAINS_FILLABLE_USERNAME_FIELD,
                         form_contains_fillable_username_field);
      logger->LogBoolean(Logger::STRING_USERNAME_FIELD_NAME_EMPTY,
                         username_field_name.empty());
      logger->LogBoolean(Logger::STRING_PASSWORD_FIELD_NAME_EMPTY,
                         password_field_name.empty());
    }

    blink::WebInputElement username_element;
    blink::WebInputElement password_element;

    if (!username_field_name.empty()) {
      const auto it = form.find(username_field_name);
      DCHECK(it != form.end());
      username_element = it->second;
    }

    if (password_field_name.empty())
      break;

    {
      const auto it = form.find(password_field_name);
      DCHECK(it != form.end());
      password_element = it->second;
    }

    blink::WebInputElement main_element =
        username_element.isNull() ? password_element : username_element;

    if (web_input_to_password_info_.find(main_element) !=
        web_input_to_password_info_.end())
      continue;

    if (!form_data.wait_for_username) {
      FillFormOnPasswordReceived(
          form_data, username_element, password_element,
          &nonscript_modified_values_,
          base::Bind(&PasswordValueGatekeeper::RegisterElement,
                     base::Unretained(&gatekeeper_)),
          logger.get());
    }

    PasswordInfo password_info;
    password_info.fill_data = form_data;
    password_info.key = key;
    password_info.password_field = password_element;
    web_input_to_password_info_[main_element] = password_info;
    if (!main_element.isPasswordField())
      password_to_username_[password_element] = username_element;
  }
}
