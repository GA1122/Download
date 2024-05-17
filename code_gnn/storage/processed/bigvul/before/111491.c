WebCore::IntRect InputHandler::rectForCaret(int index)
{
    if (!isActiveTextEdit())
        return WebCore::IntRect();

    ASSERT(m_currentFocusElement->document() && m_currentFocusElement->document()->frame());

    if (index < 0 || index > static_cast<int>(elementText().length())) {
        return WebCore::IntRect();
    }

    FrameSelection caretSelection;
    caretSelection.setSelection(DOMSupport::visibleSelectionForRangeInputElement(m_currentFocusElement.get(), index, index).visibleStart());
    caretSelection.modify(FrameSelection::AlterationExtend, DirectionForward, CharacterGranularity);
    return caretSelection.selection().visibleStart().absoluteCaretBounds();
}
