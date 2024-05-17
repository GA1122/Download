bool ThreadSafeMatch(const Vector<UChar, inlineCapacity>& vector,
                     const QualifiedName& qname) {
  return EqualIgnoringNullity(vector, qname.LocalName().Impl());
}
