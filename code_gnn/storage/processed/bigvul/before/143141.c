static Editor::Command command(Document* document, const String& commandName)
{
    LocalFrame* frame = document->frame();
    if (!frame || frame->document() != document)
        return Editor::Command();

    document->updateStyleAndLayoutTree();
    return frame->editor().createCommand(commandName, CommandFromDOM);
}
