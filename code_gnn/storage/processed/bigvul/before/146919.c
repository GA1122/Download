void Document::write(const String& text,
                     Document* entered_document,
                     ExceptionState& exception_state) {
  write(SegmentedString(text), entered_document, exception_state);
}
