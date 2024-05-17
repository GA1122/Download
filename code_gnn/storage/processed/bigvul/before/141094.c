PropertyRegistry* Document::GetPropertyRegistry() {
  if (!property_registry_ && RuntimeEnabledFeatures::CSSVariables2Enabled())
    property_registry_ = PropertyRegistry::Create();
  return property_registry_;
}
