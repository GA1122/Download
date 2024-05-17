void Document::internalAddMessage(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, PassRefPtr<ScriptCallStack> callStack, ScriptState* state)
{
    if (!isContextThread()) {
        postTask(AddConsoleMessageTask::create(source, level, message));
        return;
    }
    Page* page = this->page();
    if (!page)
        return;

    String messageURL = sourceURL;
    if (!state && sourceURL.isNull() && !lineNumber) {
        messageURL = url().string();
        if (parsing() && !isInDocumentWrite() && scriptableDocumentParser()) {
            ScriptableDocumentParser* parser = scriptableDocumentParser();
            if (!parser->isWaitingForScripts() && !parser->isExecutingScript())
                lineNumber = parser->lineNumber().oneBasedInt();
        }
    }
    page->console().addMessage(source, level, message, messageURL, lineNumber, 0, callStack, state, 0);
}
