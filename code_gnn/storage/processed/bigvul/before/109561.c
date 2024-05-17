void Document::addMessage(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, ScriptState* state)
{
    internalAddMessage(source, level, message, sourceURL, lineNumber, 0, state);
}
