void Document::logExceptionToConsole(const String& errorMessage, const String& sourceURL, int lineNumber, int columnNumber, PassRefPtr<ScriptCallStack> callStack)
{
    internalAddMessage(JSMessageSource, ErrorMessageLevel, errorMessage, sourceURL, lineNumber, callStack, 0);
}
