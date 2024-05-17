void FrameSelection::SetSelection(const SelectionInDOMTree& selection,
                                  const SetSelectionOptions& data) {
  if (SetSelectionDeprecated(selection, data))
    DidSetSelectionDeprecated(data);
}
