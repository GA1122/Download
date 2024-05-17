gfx::Size AutofillDialogViews::GetPreferredSize() const {
  if (preferred_size_.IsEmpty())
    preferred_size_ = CalculatePreferredSize(false);

  return preferred_size_;
}
