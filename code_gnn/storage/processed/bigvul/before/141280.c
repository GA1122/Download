void Document::UpdateFocusAppearanceTimerFired(TimerBase*) {
  Element* element = FocusedElement();
  if (!element)
    return;
  UpdateStyleAndLayout();
  if (element->IsFocusable())
    element->UpdateFocusAppearance(SelectionBehaviorOnFocus::kRestore);
}
