void FrameSelection::SetSelectionAndEndTyping(
    const SelectionInDOMTree& selection) {
  SetSelection(selection, SetSelectionOptions::Builder()
                              .SetShouldCloseTyping(true)
                              .SetShouldClearTypingStyle(true)
                              .Build());
}
