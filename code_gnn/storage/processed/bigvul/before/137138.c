void PasswordInputType::CountUsage() {
  CountUsageIfVisible(WebFeature::kInputTypePassword);
  if (GetElement().FastHasAttribute(HTMLNames::maxlengthAttr))
    CountUsageIfVisible(WebFeature::kInputTypePasswordMaxLength);
}
