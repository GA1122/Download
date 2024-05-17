bool HTMLInputElement::SetupDateTimeChooserParameters(
    DateTimeChooserParameters& parameters) {
  if (!GetDocument().View())
    return false;

  parameters.type = type();
  parameters.minimum = Minimum();
  parameters.maximum = Maximum();
  parameters.required = IsRequired();
  if (!RuntimeEnabledFeatures::LangAttributeAwareFormControlUIEnabled()) {
    parameters.locale = DefaultLanguage();
  } else {
    AtomicString computed_locale = ComputeInheritedLanguage();
    parameters.locale =
        computed_locale.IsEmpty() ? DefaultLanguage() : computed_locale;
  }

  StepRange step_range = CreateStepRange(kRejectAny);
  if (step_range.HasStep()) {
    parameters.step = step_range.Step().ToDouble();
    parameters.step_base = step_range.StepBase().ToDouble();
  } else {
    parameters.step = 1.0;
    parameters.step_base = 0;
  }

  parameters.anchor_rect_in_screen =
      GetDocument().View()->ContentsToScreen(PixelSnappedBoundingBox());
  parameters.double_value = input_type_->ValueAsDouble();
  parameters.is_anchor_element_rtl =
      input_type_view_->ComputedTextDirection() == TextDirection::kRtl;
  if (HTMLDataListElement* data_list = DataList()) {
    HTMLDataListOptionsCollection* options = data_list->options();
    for (unsigned i = 0; HTMLOptionElement* option = options->Item(i); ++i) {
      if (option->value().IsEmpty() || option->IsDisabledFormControl() ||
          !IsValidValue(option->value()))
        continue;
      DateTimeSuggestion suggestion;
      suggestion.value =
          input_type_->ParseToNumber(option->value(), Decimal::Nan())
              .ToDouble();
      if (std::isnan(suggestion.value))
        continue;
      suggestion.localized_value = LocalizeValue(option->value());
      suggestion.label =
          option->value() == option->label() ? String() : option->label();
      parameters.suggestions.push_back(suggestion);
    }
  }
  return true;
}
