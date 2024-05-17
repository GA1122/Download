WebTextInputInfo InputMethodController::TextInputInfo() const {
  WebTextInputInfo info;
  if (!IsAvailable())
    return info;

  if (!GetFrame().Selection().IsAvailable()) {
    return info;
  }
  Element* element = RootEditableElementOfSelection(GetFrame().Selection());
  if (!element)
    return info;

  info.input_mode = InputModeOfFocusedElement();
  info.type = TextInputType();
  info.flags = TextInputFlags();
  if (info.type == kWebTextInputTypeNone)
    return info;

  if (!GetFrame().GetEditor().CanEdit())
    return info;

  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();

  DocumentLifecycle::DisallowTransitionScope disallow_transition(
      GetDocument().Lifecycle());

  info.value = PlainText(EphemeralRange::RangeOfContents(*element),
                         TextIteratorBehavior::Builder()
                             .SetEmitsObjectReplacementCharacter(true)
                             .SetEmitsSpaceForNbsp(true)
                             .Build());

  if (info.value.IsEmpty())
    return info;

  EphemeralRange first_range = FirstEphemeralRangeOf(
      GetFrame().Selection().ComputeVisibleSelectionInDOMTreeDeprecated());
  if (first_range.IsNotNull()) {
    PlainTextRange plain_text_range(
        PlainTextRange::Create(*element, first_range));
    if (plain_text_range.IsNotNull()) {
      info.selection_start = plain_text_range.Start();
      info.selection_end = plain_text_range.End();
    }
  }

  EphemeralRange range = CompositionEphemeralRange();
  if (range.IsNotNull()) {
    PlainTextRange plain_text_range(PlainTextRange::Create(*element, range));
    if (plain_text_range.IsNotNull()) {
      info.composition_start = plain_text_range.Start();
      info.composition_end = plain_text_range.End();
    }
  }

  return info;
}
