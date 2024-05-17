String InputType::BadInputText() const {
  NOTREACHED();
  return GetLocale().QueryString(WebLocalizedString::kValidationTypeMismatch);
}
