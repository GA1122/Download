bool FormHasNonEmptyPasswordField(const FormData& form) {
  for (const auto& field : form.fields) {
    if (field.IsPasswordInputElement()) {
      if (!field.value.empty() || !field.typed_value.empty())
        return true;
    }
  }
  return false;
}
