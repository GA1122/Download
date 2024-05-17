void Document::addConsoleMessage(MessageSource source, MessageLevel level, const String& message, unsigned long requestIdentifier)
{
    if (!isContextThread()) {
        postTask(AddConsoleMessageTask::create(source, level, message));
        return;
    }

    if (Page* page = this->page())
        page->console()->addMessage(source, level, message, requestIdentifier, this);
}
