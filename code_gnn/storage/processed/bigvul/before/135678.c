void FrameSelection::SetSelection(const SelectionInDOMTree& selection,
                                  const SetSelectionData& data) {
  if (SetSelectionDeprecated(selection, data))
    DidSetSelectionDeprecated(data);
}
