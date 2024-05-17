void CSSStyleSheet::ReattachChildRuleCSSOMWrappers() {
  for (unsigned i = 0; i < child_rule_cssom_wrappers_.size(); ++i) {
    if (!child_rule_cssom_wrappers_[i])
      continue;
    child_rule_cssom_wrappers_[i]->Reattach(contents_->RuleAt(i));
  }
}
