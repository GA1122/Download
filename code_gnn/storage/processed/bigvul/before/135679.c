void FrameSelection::SetSelection(const SelectionInDOMTree& selection) {
  SetSelection(selection, SetSelectionData::Builder()
                              .SetShouldCloseTyping(true)
                              .SetShouldClearTypingStyle(true)
                              .Build());
}
