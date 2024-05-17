static void reportMessage(Document* document, MessageLevel level, const String& message)
{
    if (document->frame())
        document->addConsoleMessage(RenderingMessageSource, level, message);
}
