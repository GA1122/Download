bool RenderBlock::hasCaret(CaretType type) const
{
    bool caretBrowsing = frame()->settings() && frame()->settings()->caretBrowsingEnabled();
    RenderObject* caretPainter;
    bool isContentEditable;
    if (type == CursorCaret) {
        caretPainter = frame()->selection().caretRenderer();
        isContentEditable = frame()->selection().rendererIsEditable();
    } else {
        caretPainter = frame()->page()->dragCaretController().caretRenderer();
        isContentEditable = frame()->page()->dragCaretController().isContentEditable();
    }
    return caretPainter == this && (isContentEditable || caretBrowsing);
}
