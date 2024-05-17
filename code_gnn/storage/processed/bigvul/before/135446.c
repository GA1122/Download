void Document::writeln(const String& text, Document* enteredDocument, ExceptionState& exceptionState)
{
    write(text, enteredDocument, exceptionState);
    if (exceptionState.hadException())
        return;
    write("\n", enteredDocument);
}
