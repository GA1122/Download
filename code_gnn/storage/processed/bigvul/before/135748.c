bool InputMethodController::InsertText(const String& text) {
  if (DispatchBeforeInputInsertText(GetDocument().FocusedElement(), text) !=
      DispatchEventResult::kNotCanceled)
    return false;
  GetEditor().InsertText(text, 0);
  return true;
}
