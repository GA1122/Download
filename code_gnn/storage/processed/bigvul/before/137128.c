std::pair<String, String> InputType::ValidationMessage(
    const InputTypeView& input_type_view) const {
  const String value = GetElement().value();

  if (input_type_view.HasBadInput())
    return std::make_pair(BadInputText(), g_empty_string);

  if (ValueMissing(value))
    return std::make_pair(ValueMissingText(), g_empty_string);

  if (TypeMismatch())
    return std::make_pair(TypeMismatchText(), g_empty_string);

  if (PatternMismatch(value)) {
    return std::make_pair(
        GetLocale().QueryString(WebLocalizedString::kValidationPatternMismatch),
        GetElement().FastGetAttribute(titleAttr).GetString());
  }

  if (GetElement().TooLong()) {
    return std::make_pair(GetLocale().ValidationMessageTooLongText(
                              value.length(), GetElement().maxLength()),
                          g_empty_string);
  }

  if (GetElement().TooShort()) {
    return std::make_pair(GetLocale().ValidationMessageTooShortText(
                              value.length(), GetElement().minLength()),
                          g_empty_string);
  }

  if (!IsSteppable())
    return std::make_pair(g_empty_string, g_empty_string);

  const Decimal numeric_value = ParseToNumberOrNaN(value);
  if (!numeric_value.IsFinite())
    return std::make_pair(g_empty_string, g_empty_string);

  StepRange step_range(CreateStepRange(kRejectAny));

  if (numeric_value < step_range.Minimum())
    return std::make_pair(RangeUnderflowText(step_range.Minimum()),
                          g_empty_string);

  if (numeric_value > step_range.Maximum())
    return std::make_pair(RangeOverflowText(step_range.Maximum()),
                          g_empty_string);

  if (step_range.StepMismatch(numeric_value)) {
    DCHECK(step_range.HasStep());
    Decimal candidate1 = step_range.ClampValue(numeric_value);
    String localized_candidate1 = LocalizeValue(Serialize(candidate1));
    Decimal candidate2 = candidate1 < numeric_value
                             ? candidate1 + step_range.Step()
                             : candidate1 - step_range.Step();
    if (!candidate2.IsFinite() || candidate2 < step_range.Minimum() ||
        candidate2 > step_range.Maximum()) {
      return std::make_pair(
          GetLocale().QueryString(
              WebLocalizedString::kValidationStepMismatchCloseToLimit,
              localized_candidate1),
          g_empty_string);
    }
    String localized_candidate2 = LocalizeValue(Serialize(candidate2));
    if (candidate1 < candidate2) {
      return std::make_pair(
          GetLocale().QueryString(WebLocalizedString::kValidationStepMismatch,
                                  localized_candidate1, localized_candidate2),
          g_empty_string);
    }
    return std::make_pair(
        GetLocale().QueryString(WebLocalizedString::kValidationStepMismatch,
                                localized_candidate2, localized_candidate1),
        g_empty_string);
  }

  return std::make_pair(g_empty_string, g_empty_string);
}
