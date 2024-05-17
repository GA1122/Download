void RenderBlock::paintCaret(PaintInfo& paintInfo, const LayoutPoint& paintOffset, CaretType type)
{
    if (!hasCaret(type))
        return;

    if (type == CursorCaret)
        frame()->selection().paintCaret(paintInfo.context, paintOffset, paintInfo.rect);
    else
        frame()->page()->dragCaretController().paintDragCaret(frame(), paintInfo.context, paintOffset, paintInfo.rect);
}
