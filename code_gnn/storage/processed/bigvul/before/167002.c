CSSStyleSheet::InspectorMutationScope::~InspectorMutationScope() {
  style_sheet_->DisableRuleAccessForInspector();
}
