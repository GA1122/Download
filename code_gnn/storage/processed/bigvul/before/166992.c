CSSRuleList* CSSStyleSheet::cssRules(ExceptionState& exception_state) {
  if (!CanAccessRules()) {
    exception_state.ThrowSecurityError("Cannot access rules");
    return nullptr;
  }
  if (!rule_list_cssom_wrapper_)
    rule_list_cssom_wrapper_ = StyleSheetCSSRuleList::Create(this);
  return rule_list_cssom_wrapper_.Get();
}
