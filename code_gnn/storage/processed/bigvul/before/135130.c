void Document::addConsoleMessage(PassRefPtrWillBeRawPtr<ConsoleMessage> consoleMessage)
{
    if (!isContextThread()) {
        m_taskRunner->postTask(BLINK_FROM_HERE, AddConsoleMessageTask::create(consoleMessage->source(), consoleMessage->level(), consoleMessage->message()));
        return;
    }

    if (!m_frame)
        return;

    if (!consoleMessage->scriptState() && consoleMessage->url().isNull() && !consoleMessage->lineNumber()) {
        consoleMessage->setURL(url().string());
        if (!isInDocumentWrite() && scriptableDocumentParser()) {
            ScriptableDocumentParser* parser = scriptableDocumentParser();
            if (parser->isParsingAtLineNumber())
                consoleMessage->setLineNumber(parser->lineNumber().oneBasedInt());
        }
    }
    m_frame->console().addMessage(consoleMessage);
}
