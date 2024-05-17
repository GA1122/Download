void CSSStyleSheet::WillMutateRules() {
  if (!contents_->IsUsedFromTextCache() &&
      !contents_->IsReferencedFromResource()) {
    contents_->ClearRuleSet();
    contents_->SetMutable();
    return;
  }
  DCHECK(contents_->IsCacheableForStyleElement() ||
         contents_->IsCacheableForResource());

  contents_->UnregisterClient(this);
  contents_ = contents_->Copy();
  contents_->RegisterClient(this);

  contents_->SetMutable();

  ReattachChildRuleCSSOMWrappers();
}
