static Editor::Command command(Document* document, const String& commandName)
{
    LocalFrame* frame = document->frame();
    if (!frame || frame->document() != document)
        return Editor::Command();

    document->updateLayoutTreeIfNeeded();
    return frame->editor().command(commandName, CommandFromDOM);
}
