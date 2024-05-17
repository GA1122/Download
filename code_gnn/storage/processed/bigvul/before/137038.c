void InputType::AppendToFormData(FormData& form_data) const {
  form_data.append(GetElement().GetName(), GetElement().value());
}
