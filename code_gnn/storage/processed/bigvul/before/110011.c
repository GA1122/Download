void HTMLSelectElement::dispatchFocusEvent(PassRefPtr<Node> oldFocusedNode, FocusDirection direction)
{
    if (usesMenuList())
        saveLastSelection();
    HTMLFormControlElementWithState::dispatchFocusEvent(oldFocusedNode, direction);
}
