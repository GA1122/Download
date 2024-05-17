CSSRule* CSSStyleSheet::item(unsigned index) {
  unsigned rule_count = length();
  if (index >= rule_count)
    return nullptr;

  if (child_rule_cssom_wrappers_.IsEmpty())
    child_rule_cssom_wrappers_.Grow(rule_count);
  DCHECK_EQ(child_rule_cssom_wrappers_.size(), rule_count);

  TraceWrapperMember<CSSRule>& css_rule = child_rule_cssom_wrappers_[index];
  if (!css_rule)
    css_rule = contents_->RuleAt(index)->CreateCSSOMWrapper(this);
  return css_rule.Get();
}
