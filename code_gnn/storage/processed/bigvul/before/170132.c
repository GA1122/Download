bool FillFormOnPasswordReceived(
    const PasswordFormFillData& fill_data,
    blink::WebInputElement username_element,
    blink::WebInputElement password_element,
    std::map<const blink::WebInputElement, blink::WebString>*
        nonscript_modified_values,
    base::Callback<void(blink::WebInputElement*)> registration_callback,
    RendererSavePasswordProgressLogger* logger) {
  blink::WebFrame* cur_frame = password_element.document().frame();
  blink::WebString bottom_frame_origin =
      cur_frame->getSecurityOrigin().toString();

  DCHECK(cur_frame);

  while (cur_frame->parent()) {
    cur_frame = cur_frame->parent();
    if (!bottom_frame_origin.equals(cur_frame->getSecurityOrigin().toString()))
      return false;
  }

  if (!IsElementAutocompletable(password_element))
    return false;

  bool form_contains_fillable_username_field =
      FillDataContainsFillableUsername(fill_data);
  bool ambiguous_or_empty_names =
      DoesFormContainAmbiguousOrEmptyNames(fill_data);
  base::string16 username_field_name;
  if (form_contains_fillable_username_field)
    username_field_name =
        FieldName(fill_data.username_field, ambiguous_or_empty_names);

  bool form_has_fillable_username = !username_field_name.empty() &&
                                    IsElementAutocompletable(username_element);

  if (ShouldFillOnAccountSelect()) {
    if (!ShouldHighlightFields()) {
      return false;
    }

    if (form_has_fillable_username) {
      username_element.setAutofilled(true);
    } else if (username_element.isNull() ||
               HasExactMatchSuggestion(fill_data, username_element.value())) {
      password_element.setAutofilled(true);
    }
    return false;
  }

  if (form_has_fillable_username && username_element.value().isEmpty()) {
    username_element.setValue(fill_data.username_field.value, true);
  }

  return FillUserNameAndPassword(
      &username_element, &password_element, fill_data,
      true  , false  ,
      nonscript_modified_values, registration_callback, logger);
}
