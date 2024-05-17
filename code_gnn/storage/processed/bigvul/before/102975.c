void TabStripModel::InsertTabContentsAt(int index,
                                        TabContentsWrapper* contents,
                                        int add_types) {
  bool active = add_types & ADD_ACTIVE;
  bool pin =
      contents->extension_tab_helper()->is_app() || add_types & ADD_PINNED;
  index = ConstrainInsertionIndex(index, pin);

  closing_all_ = false;

  TabContentsWrapper* selected_contents = GetSelectedTabContents();
  TabContentsData* data = new TabContentsData(contents);
  data->pinned = pin;
  if ((add_types & ADD_INHERIT_GROUP) && selected_contents) {
    if (active) {
      ForgetAllOpeners();
    }
    data->SetGroup(&selected_contents->controller());
  } else if ((add_types & ADD_INHERIT_OPENER) && selected_contents) {
    if (active) {
      ForgetAllOpeners();
    }
    data->opener = &selected_contents->controller();
  }

  contents_data_.insert(contents_data_.begin() + index, data);

  selection_model_.IncrementFrom(index);

  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabInsertedAt(contents, index, active));

  if (active) {
    selection_model_.SetSelectedIndex(index);
    NotifyTabSelectedIfChanged(selected_contents, index, false);
  }
}
