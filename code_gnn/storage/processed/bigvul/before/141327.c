String Document::domain() const {
  return GetSecurityOrigin()->Domain();
}
