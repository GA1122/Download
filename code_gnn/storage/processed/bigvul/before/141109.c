bool Document::HasValidNamespaceForAttributes(const QualifiedName& q_name) {
  return HasValidNamespaceForElements(q_name);
}
