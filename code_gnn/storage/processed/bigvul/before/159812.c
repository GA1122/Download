static bool HasName(const HTMLToken& token, const QualifiedName& name) {
  return ThreadSafeMatch(token.GetName(), name);
}
