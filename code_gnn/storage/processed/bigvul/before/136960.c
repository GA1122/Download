void HTMLInputElement::SetEditingValue(const String& value) {
  if (!GetLayoutObject() || !IsTextField())
    return;
  SetInnerEditorValue(value);
  SubtreeHasChanged();

  unsigned max = value.length();
  SetSelectionRange(max, max);
  DispatchInputEvent();
}
