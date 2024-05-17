CSSStyleSheet::InspectorMutationScope::InspectorMutationScope(
    CSSStyleSheet* sheet)
    : style_sheet_(sheet) {
  style_sheet_->EnableRuleAccessForInspector();
}
