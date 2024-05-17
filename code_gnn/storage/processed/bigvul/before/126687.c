  void PrepareTabstripForSelectionTest(TabStripModel* model,
                                       int tab_count,
                                       int pinned_count,
                                       const std::string& selected_tabs) {
    for (int i = 0; i < tab_count; ++i) {
      TabContents* contents = CreateTabContents();
      SetID(contents->web_contents(), i);
      model->AppendTabContents(contents, true);
    }
    for (int i = 0; i < pinned_count; ++i)
      model->SetTabPinned(i, true);

    TabStripSelectionModel selection_model;
    std::vector<std::string> selection;
    base::SplitStringAlongWhitespace(selected_tabs, &selection);
    for (size_t i = 0; i < selection.size(); ++i) {
      int value;
      ASSERT_TRUE(base::StringToInt(selection[i], &value));
      selection_model.AddIndexToSelection(value);
    }
    selection_model.set_active(selection_model.selected_indices()[0]);
    model->SetSelectionFromModel(selection_model);
  }
