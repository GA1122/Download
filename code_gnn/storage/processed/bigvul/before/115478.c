void InjectedBundlePage::willAddMessageToConsole(WKStringRef message, uint32_t lineNumber)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    WTF::String messageString = toWTFString(message);
    size_t nullCharPos = messageString.find(UChar(0));
    if (nullCharPos != WTF::notFound)
        messageString.truncate(nullCharPos);

    size_t fileProtocolStart = messageString.find("file://");
    if (fileProtocolStart != WTF::notFound)
        messageString = messageString.substring(0, fileProtocolStart) + lastFileURLPathComponent(messageString.substring(fileProtocolStart));

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("CONSOLE MESSAGE: ");
    if (lineNumber) {
        stringBuilder.appendLiteral("line ");
        stringBuilder.appendNumber(lineNumber);
        stringBuilder.appendLiteral(": ");
    }
    stringBuilder.append(messageString);
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
