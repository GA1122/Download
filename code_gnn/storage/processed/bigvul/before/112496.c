void Document::logExceptionToConsole(const String& errorMessage, const String& sourceURL, int lineNumber, PassRefPtr<ScriptCallStack> callStack)
{
    addMessage(JSMessageSource, ErrorMessageLevel, errorMessage, sourceURL, lineNumber, callStack);
}
