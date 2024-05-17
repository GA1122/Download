void Document::writeln(const String& text, Document* ownerDocument, ExceptionState& exceptionState)
{
    write(text, ownerDocument, exceptionState);
    if (exceptionState.hadException())
        return;
    write("\n", ownerDocument);
}
