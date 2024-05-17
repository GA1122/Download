static IntPoint getLocationForKeyboardGeneratedContextMenu(Frame* frame)
{
    FrameSelection* selection = frame->selection();
    if (!selection->selection().isNonOrphanedCaretOrRange()
         || (selection->selection().isCaret() && !selection->selection().isContentEditable())) {
        if (Node* focusedNode = getFocusedNode(frame))
            return focusedNode->getRect().location();

        return IntPoint(gContextMenuMargin, gContextMenuMargin);
    }

    IntRect firstRect = frame->editor()->firstRectForRange(selection->selection().firstRange().get());
    return IntPoint(firstRect.x(), firstRect.maxY());
}
