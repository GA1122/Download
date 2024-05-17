void Document::writeln(LocalDOMWindow* callingWindow, const Vector<String>& text, ExceptionState& exceptionState)
{
    DCHECK(callingWindow);
    StringBuilder builder;
    for (const String& string : text)
        builder.append(string);
    writeln(builder.toString(), callingWindow->document(), exceptionState);
}
