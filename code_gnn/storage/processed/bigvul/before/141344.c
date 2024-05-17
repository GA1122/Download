Location* Document::location() const {
  if (!GetFrame())
    return nullptr;

  return domWindow()->location();
}
