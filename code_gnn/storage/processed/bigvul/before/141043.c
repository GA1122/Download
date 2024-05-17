AtomicString Document::EncodingName() const {
  return AtomicString(Encoding().GetName());
}
