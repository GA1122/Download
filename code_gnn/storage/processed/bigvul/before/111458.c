bool InputHandler::deleteSelection()
{
    if (!isActiveTextEdit())
        return false;

    ASSERT(m_currentFocusElement->document() && m_currentFocusElement->document()->frame());
    Frame* frame = m_currentFocusElement->document()->frame();

    if (frame->selection()->selectionType() != VisibleSelection::RangeSelection)
        return false;

    ASSERT(frame->editor());
    return handleKeyboardInput(Platform::KeyboardEvent(KEYCODE_BACKSPACE, Platform::KeyboardEvent::KeyDown, 0), false  );
}
