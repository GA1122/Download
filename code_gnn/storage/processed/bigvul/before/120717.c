void BaseMultipleFieldsDateAndTimeInputType::handleFocusEvent(Node* oldFocusedNode, FocusDirection direction)
{
    DateTimeEditElement* edit = dateTimeEditElement();
    if (!edit || m_isDestroyingShadowSubtree)
        return;
    if (direction == FocusDirectionBackward) {
        if (element()->document()->page())
            element()->document()->page()->focusController()->advanceFocus(direction, 0);
    } else if (direction == FocusDirectionNone || direction == FocusDirectionMouse) {
        edit->focusByOwner(oldFocusedNode);
    } else
        edit->focusByOwner();
}
