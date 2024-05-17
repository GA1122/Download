PasswordAutofillAgent::ExtractFormStructureInfo(const FormData& form_data) {
  FormStructureInfo result;
  result.unique_renderer_id = form_data.unique_renderer_id;
  result.fields.resize(form_data.fields.size());

  for (size_t i = 0; i < form_data.fields.size(); ++i) {
    const FormFieldData& form_field = form_data.fields[i];

    FormFieldInfo& field_info = result.fields[i];
    field_info.unique_renderer_id = form_field.unique_renderer_id;
    field_info.form_control_type = form_field.form_control_type;
    field_info.autocomplete_attribute = form_field.autocomplete_attribute;
    field_info.is_focusable = form_field.is_focusable;
  }

  return result;
}
