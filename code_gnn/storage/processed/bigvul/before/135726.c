EphemeralRange InputMethodController::CompositionEphemeralRange() const {
  if (!HasComposition())
    return EphemeralRange();
  return EphemeralRange(composition_range_.Get());
}
