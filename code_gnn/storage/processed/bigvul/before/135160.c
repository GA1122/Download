void Document::clearFocusedElement()
{
    setFocusedElement(nullptr, FocusParams(SelectionBehaviorOnFocus::None, WebFocusTypeNone, nullptr));
}
