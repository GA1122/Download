void Document::ClearFocusedElement() {
  SetFocusedElement(nullptr, FocusParams(SelectionBehaviorOnFocus::kNone,
                                         kWebFocusTypeNone, nullptr));
}
