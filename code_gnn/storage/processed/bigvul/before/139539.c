bool ModifySelectionyWithPageGranularity(
    LocalFrame& frame,
    SelectionModifyAlteration alter,
    unsigned vertical_distance,
    SelectionModifyVerticalDirection direction) {
  SelectionModifier selection_modifier(
      frame, frame.Selection().ComputeVisibleSelectionInDOMTree());
  if (!selection_modifier.ModifyWithPageGranularity(alter, vertical_distance,
                                                    direction)) {
    return false;
  }

  frame.Selection().SetSelection(selection_modifier.Selection().AsSelection(),
                                 FrameSelection::kCloseTyping |
                                     FrameSelection::kClearTypingStyle |
                                     FrameSelection::kUserTriggered,
                                 alter == SelectionModifyAlteration::kMove
                                     ? CursorAlignOnScroll::kAlways
                                     : CursorAlignOnScroll::kIfNeeded);
  return true;
}
