void AutocompleteEditModel::PopupBoundsChangedTo(const gfx::Rect& bounds) {
  InstantController* instant = controller_->GetInstant();
  if (instant)
    instant->SetOmniboxBounds(bounds);
}
