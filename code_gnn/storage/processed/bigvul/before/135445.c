void Document::write(LocalDOMWindow* callingWindow, const Vector<String>& text, ExceptionState& exceptionState)
{
    ASSERT(callingWindow);
    StringBuilder builder;
    for (const String& string : text)
        builder.append(string);
    write(builder.toString(), callingWindow->document(), exceptionState);
}
