void Document::addConsoleMessage(ConsoleMessage* consoleMessage)
{
    if (!isContextThread()) {
        m_taskRunner->postTask(BLINK_FROM_HERE, AddConsoleMessageTask::create(consoleMessage->source(), consoleMessage->level(), consoleMessage->message()));
        return;
    }

    if (!m_frame)
        return;

    if (!consoleMessage->messageId() && !consoleMessage->relatedMessageId() && consoleMessage->location()->isUnknown()) {
        unsigned lineNumber = 0;
        if (!isInDocumentWrite() && scriptableDocumentParser()) {
            ScriptableDocumentParser* parser = scriptableDocumentParser();
            if (parser->isParsingAtLineNumber())
                lineNumber = parser->lineNumber().oneBasedInt();
        }
        consoleMessage = ConsoleMessage::create(consoleMessage->source(), consoleMessage->level(), consoleMessage->message(), SourceLocation::create(url().getString(), lineNumber, 0, nullptr), consoleMessage->scriptArguments());
    }
    m_frame->console().addMessage(consoleMessage);
}
