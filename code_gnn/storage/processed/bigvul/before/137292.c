bool Textfield::SetSelectionRange(const gfx::Range& range) {
  if (!ImeEditingAllowed() || !range.IsValid())
    return false;
  OnBeforeUserAction();
  SelectRange(range);
  OnAfterUserAction();
  return true;
}
