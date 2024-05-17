static TriState StateTextWritingDirection(LocalFrame& frame,
                                          WritingDirection direction) {
  frame.GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  bool has_nested_or_multiple_embeddings;
  WritingDirection selection_direction = TextDirectionForSelection(
      frame.Selection().ComputeVisibleSelectionInDOMTreeDeprecated(),
      frame.GetEditor().TypingStyle(), has_nested_or_multiple_embeddings);
  return (selection_direction == direction &&
          !has_nested_or_multiple_embeddings)
             ? kTrueTriState
             : kFalseTriState;
}
