Document* BaseAudioContext::GetDocument() const {
  return ToDocument(GetExecutionContext());
}
