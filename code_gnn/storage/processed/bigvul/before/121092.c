void BaseMultipleFieldsDateAndTimeInputType::handleFocusEvent(Node* oldFocusedNode, FocusDirection direction)
{
    if (!m_dateTimeEditElement)
        return;
    if (direction == FocusDirectionBackward) {
        if (element()->document()->page())
            element()->document()->page()->focusController()->advanceFocus(direction, 0);
    } else if (direction == FocusDirectionNone) {
        m_dateTimeEditElement->focusByOwner(oldFocusedNode);
    } else
        m_dateTimeEditElement->focusByOwner();
}
