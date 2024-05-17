bool Editor::smartInsertDeleteEnabled() const {
  if (Settings* settings = frame().settings())
    return settings->getSmartInsertDeleteEnabled();
  return false;
}
