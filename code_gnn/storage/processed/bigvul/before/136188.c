void Document::write(const String& text, Document* ownerDocument, ExceptionState& exceptionState)
{
    write(SegmentedString(text), ownerDocument, exceptionState);
}
