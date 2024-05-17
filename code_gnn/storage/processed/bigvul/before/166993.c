void CSSStyleSheet::deleteRule(unsigned index,
                               ExceptionState& exception_state) {
  if (!CanAccessRules()) {
    exception_state.ThrowSecurityError(
        "Cannot access StyleSheet to deleteRule");
    return;
  }

  DCHECK(child_rule_cssom_wrappers_.IsEmpty() ||
         child_rule_cssom_wrappers_.size() == contents_->RuleCount());

  if (index >= length()) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kIndexSizeError,
        "The index provided (" + String::Number(index) +
            ") is larger than the maximum index (" +
            String::Number(length() - 1) + ").");
    return;
  }
  RuleMutationScope mutation_scope(this);

  bool success = contents_->WrapperDeleteRule(index);
  if (!success) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "Failed to delete rule");
    return;
  }

  if (!child_rule_cssom_wrappers_.IsEmpty()) {
    if (child_rule_cssom_wrappers_[index])
      child_rule_cssom_wrappers_[index]->SetParentStyleSheet(nullptr);
    child_rule_cssom_wrappers_.EraseAt(index);
  }
}
