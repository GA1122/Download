bool Document::execCommand(const String& commandName, bool, const String& value)
{
    static bool inExecCommand = false;
    if (inExecCommand) {
        String message = "We don't execute document.execCommand() this time, because it is called recursively.";
        addConsoleMessage(ConsoleMessage::create(JSMessageSource, WarningMessageLevel, message));
        return false;
    }
    TemporaryChange<bool> executeScope(inExecCommand, true);

    EventQueueScope eventQueueScope;
    Editor::Command editorCommand = command(this, commandName);
    Platform::current()->histogramSparse("WebCore.Document.execCommand", editorCommand.idForHistogram());
    return editorCommand.execute(value);
}
