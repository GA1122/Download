static PositionInFlatTree AdjustPositionRespectUserSelectAll(
    Node* inner_node,
    const PositionInFlatTree& selection_start,
    const PositionInFlatTree& selection_end,
    const PositionInFlatTree& position) {
  const VisibleSelectionInFlatTree& selection_in_user_select_all =
      CreateVisibleSelection(ExpandSelectionToRespectUserSelectAll(
          inner_node,
          position.IsNull()
              ? SelectionInFlatTree()
              : SelectionInFlatTree::Builder().Collapse(position).Build()));
  if (!selection_in_user_select_all.IsRange())
    return position;
  if (selection_in_user_select_all.Start().CompareTo(selection_start) < 0)
    return selection_in_user_select_all.Start();
  if (selection_end.CompareTo(selection_in_user_select_all.End()) < 0)
    return selection_in_user_select_all.End();
  return position;
}
