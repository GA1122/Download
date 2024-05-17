PasswordAutofillAgent::FindUsernamePasswordElements(
    const PasswordFormFillData& form_data) {
  const uint32_t username_renderer_id =
      form_data.username_field.unique_renderer_id;
  const uint32_t password_renderer_id =
      form_data.password_field.unique_renderer_id;
  const bool is_username_present =
      username_renderer_id != FormFieldData::kNotSetFormControlRendererId;
  const bool is_password_present =
      password_renderer_id != FormFieldData::kNotSetFormControlRendererId;

  std::vector<uint32_t> element_ids;
  if (is_password_present)
    element_ids.push_back(password_renderer_id);
  if (is_username_present)
    element_ids.push_back(username_renderer_id);

  WebDocument doc = render_frame()->GetWebFrame()->GetDocument();
  bool wrapped_in_form_tag =
      form_data.form_renderer_id != FormData::kNotSetFormRendererId;
  std::vector<WebFormControlElement> elements =
      wrapped_in_form_tag
          ? form_util::FindFormControlElementsByUniqueRendererId(
                doc, form_data.form_renderer_id, element_ids)
          : form_util::FindFormControlElementsByUniqueRendererId(doc,
                                                                 element_ids);

  WebInputElement password_field;
  size_t current_index = 0;
  if (is_password_present)
    password_field = ConvertToWebInput(elements[current_index++]);

  WebInputElement username_field;
  if (is_username_present)
    username_field = ConvertToWebInput(elements[current_index++]);

  return std::make_pair(username_field, password_field);
}
