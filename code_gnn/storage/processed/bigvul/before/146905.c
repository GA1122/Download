String Document::origin() const {
  return GetSecurityOrigin()->ToString();
}
