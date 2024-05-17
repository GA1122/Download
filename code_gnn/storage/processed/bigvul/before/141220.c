void Document::SetCSSTarget(Element* new_target) {
  if (css_target_)
    css_target_->PseudoStateChanged(CSSSelector::kPseudoTarget);
  css_target_ = new_target;
  if (css_target_)
    css_target_->PseudoStateChanged(CSSSelector::kPseudoTarget);
}
