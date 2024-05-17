EphemeralRange InputMethodController::EphemeralRangeForOffsets(
    const PlainTextRange& offsets) const {
  if (offsets.IsNull())
    return EphemeralRange();
  Element* root_editable_element =
      GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .RootEditableElement();
  if (!root_editable_element)
    return EphemeralRange();

  DCHECK(!GetDocument().NeedsLayoutTreeUpdate());

  return offsets.CreateRange(*root_editable_element);
}
