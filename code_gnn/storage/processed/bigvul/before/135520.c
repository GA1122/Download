bool Editor::CanCut() const {
  return CanCopy() && CanDelete();
}
