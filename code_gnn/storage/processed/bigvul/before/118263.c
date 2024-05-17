void AutofillDialogViews::ModelChanged() {
  menu_runner_.reset();

  for (DetailGroupMap::const_iterator iter = detail_groups_.begin();
       iter != detail_groups_.end(); ++iter) {
    UpdateDetailsGroupState(iter->second);
  }
}
