int InputHandler::selectionPosition(bool start) const
{
    if (!m_currentFocusElement->document() || !m_currentFocusElement->document()->frame())
        return 0;

    if (HTMLTextFormControlElement* controlElement = DOMSupport::toTextControlElement(m_currentFocusElement.get()))
        return start ? controlElement->selectionStart() : controlElement->selectionEnd();

    FrameSelection caretSelection;
    caretSelection.setSelection(m_currentFocusElement->document()->frame()->selection()->selection());
    RefPtr<Range> rangeSelection = caretSelection.selection().toNormalizedRange();
    if (!rangeSelection)
        return 0;

    int selectionPointInNode = start ? rangeSelection->startOffset() : rangeSelection->endOffset();
    Node* containerNode = start ? rangeSelection->startContainer() : rangeSelection->endContainer();

    ExceptionCode ec;
    RefPtr<Range> rangeForNode = rangeOfContents(m_currentFocusElement.get());
    rangeForNode->setEnd(containerNode, selectionPointInNode, ec);
    ASSERT(!ec);

    return TextIterator::rangeLength(rangeForNode.get());
}
