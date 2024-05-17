Location* Document::location() const {
  if (!GetFrame())
    return 0;

  return domWindow()->location();
}
