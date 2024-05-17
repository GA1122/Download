void TabStripModel::MoveTabContentsAtImpl(int index,
                                          int to_position,
                                          bool select_after_move) {
  WebContentsData* moved_data = contents_data_[index];
  contents_data_.erase(contents_data_.begin() + index);
  contents_data_.insert(contents_data_.begin() + to_position, moved_data);

  selection_model_.Move(index, to_position);
  if (!selection_model_.IsSelected(select_after_move) && select_after_move) {
    selection_model_.SetSelectedIndex(to_position);
  }

  TabContents* tab_contents =
      TabContents::FromWebContents(moved_data->contents);
  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabMoved(tab_contents, index, to_position));
}
