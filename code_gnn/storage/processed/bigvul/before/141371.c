KURL Document::urlForBinding() const {
  if (!Url().IsNull()) {
    return Url();
  }
  return BlankURL();
}
