unsigned CSSStyleSheet::insertRule(const String& rule_string,
                                   unsigned index,
                                   ExceptionState& exception_state) {
  if (!CanAccessRules()) {
    exception_state.ThrowSecurityError(
        "Cannot access StyleSheet to insertRule");
    return 0;
  }

  DCHECK(child_rule_cssom_wrappers_.IsEmpty() ||
         child_rule_cssom_wrappers_.size() == contents_->RuleCount());

  if (index > length()) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kIndexSizeError,
        "The index provided (" + String::Number(index) +
            ") is larger than the maximum index (" + String::Number(length()) +
            ").");
    return 0;
  }
  const CSSParserContext* context =
      CSSParserContext::CreateWithStyleSheet(contents_->ParserContext(), this);
  StyleRuleBase* rule =
      CSSParser::ParseRule(context, contents_.Get(), rule_string);

  if (!rule) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kSyntaxError,
        "Failed to parse the rule '" + rule_string + "'.");
    return 0;
  }
  RuleMutationScope mutation_scope(this);
  bool success = contents_->WrapperInsertRule(rule, index);
  if (!success) {
    if (rule->IsNamespaceRule())
      exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                        "Failed to insert the rule");
    else
      exception_state.ThrowDOMException(
          DOMExceptionCode::kHierarchyRequestError,
          "Failed to insert the rule.");
    return 0;
  }
  if (!child_rule_cssom_wrappers_.IsEmpty())
    child_rule_cssom_wrappers_.insert(index, Member<CSSRule>(nullptr));

  return index;
}
