void Editor::ChangeSelectionAfterCommand(
    const SelectionInDOMTree& new_selection,
    FrameSelection::SetSelectionOptions options) {
  if (new_selection.IsNone())
    return;

  bool selection_did_not_change_dom_position =
      new_selection == GetFrame().Selection().GetSelectionInDOMTree();
  GetFrame().Selection().SetSelection(
      SelectionInDOMTree::Builder(new_selection)
          .SetIsHandleVisible(GetFrame().Selection().IsHandleVisible())
          .Build(),
      options);

  if (selection_did_not_change_dom_position) {
    Client().RespondToChangedSelection(
        frame_, GetFrame().Selection().GetSelectionInDOMTree().Type());
  }
}
