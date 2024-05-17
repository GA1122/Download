void InputType::InRangeChanged() const {
  if (IsSteppable()) {
    GetElement().PseudoStateChanged(CSSSelector::kPseudoInRange);
    GetElement().PseudoStateChanged(CSSSelector::kPseudoOutOfRange);
  }
}
