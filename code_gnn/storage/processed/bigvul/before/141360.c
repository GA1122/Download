void Document::setDesignMode(const String& value) {
  bool new_value = design_mode_;
  if (DeprecatedEqualIgnoringCase(value, "on")) {
    new_value = true;
    UseCounter::Count(*this, WebFeature::kDocumentDesignModeEnabeld);
  } else if (DeprecatedEqualIgnoringCase(value, "off")) {
    new_value = false;
  }
  if (new_value == design_mode_)
    return;
  design_mode_ = new_value;
  SetNeedsStyleRecalc(
      kSubtreeStyleChange,
      StyleChangeReasonForTracing::Create(style_change_reason::kDesignMode));
}
