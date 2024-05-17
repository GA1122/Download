void Document::logExceptionToConsole(const String& errorMessage, std::unique_ptr<SourceLocation> location)
{
    ConsoleMessage* consoleMessage = ConsoleMessage::create(JSMessageSource, ErrorMessageLevel, errorMessage, std::move(location));
    addConsoleMessage(consoleMessage);
}
