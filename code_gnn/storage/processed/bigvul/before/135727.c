Range* InputMethodController::CompositionRange() const {
  return HasComposition() ? composition_range_ : nullptr;
}
