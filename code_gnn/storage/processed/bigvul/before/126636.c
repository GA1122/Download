void TabStripModel::InsertTabContentsAt(int index,
                                        TabContents* contents,
                                        int add_types) {
  bool active = add_types & ADD_ACTIVE;
  extensions::TabHelper* extensions_tab_helper =
      extensions::TabHelper::FromWebContents(contents->web_contents());
  bool pin = extensions_tab_helper->is_app() || add_types & ADD_PINNED;
  index = ConstrainInsertionIndex(index, pin);

  closing_all_ = false;

  WebContents* active_contents = GetActiveWebContents();
  WebContentsData* data = new WebContentsData(contents->web_contents());
  data->pinned = pin;
  if ((add_types & ADD_INHERIT_GROUP) && active_contents) {
    if (active) {
      ForgetAllOpeners();
    }
    data->SetGroup(active_contents);
  } else if ((add_types & ADD_INHERIT_OPENER) && active_contents) {
    if (active) {
      ForgetAllOpeners();
    }
    data->opener = active_contents;
  }

  contents_data_.insert(contents_data_.begin() + index, data);

  selection_model_.IncrementFrom(index);

  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabInsertedAt(contents->web_contents(), index, active));
  if (active) {
    TabStripSelectionModel new_model;
    new_model.Copy(selection_model_);
    new_model.SetSelectedIndex(index);
    SetSelection(new_model, NOTIFY_DEFAULT);
  }
}
