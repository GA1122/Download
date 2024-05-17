void Document::SetCompatibilityMode(CompatibilityMode mode) {
  if (compatibility_mode_locked_ || mode == compatibility_mode_)
    return;

  if (compatibility_mode_ == kQuirksMode)
    UseCounter::Count(*this, WebFeature::kQuirksModeDocument);
  else if (compatibility_mode_ == kLimitedQuirksMode)
    UseCounter::Count(*this, WebFeature::kLimitedQuirksModeDocument);

  compatibility_mode_ = mode;
  GetSelectorQueryCache().Invalidate();
}
