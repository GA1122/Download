void TabStripModel::MoveTabContentsAt(int index,
                                      int to_position,
                                      bool select_after_move) {
  DCHECK(ContainsIndex(index));
  if (index == to_position)
    return;

  int first_non_mini_tab = IndexOfFirstNonMiniTab();
  if ((index < first_non_mini_tab && to_position >= first_non_mini_tab) ||
      (to_position < first_non_mini_tab && index >= first_non_mini_tab)) {
    return;
  }

  MoveTabContentsAtImpl(index, to_position, select_after_move);
}
