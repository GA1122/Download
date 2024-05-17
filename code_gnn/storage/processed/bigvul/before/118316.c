ServerFieldType AutofillDialogViews::TypeForCombobox(
    const views::Combobox* combobox) const {
  for (DetailGroupMap::const_iterator it = detail_groups_.begin();
       it != detail_groups_.end(); ++it) {
    const DetailsGroup& group = it->second;
    if (!delegate_->SectionIsActive(group.section))
      continue;

    for (ComboboxMap::const_iterator combo_it = group.comboboxes.begin();
         combo_it != group.comboboxes.end(); ++combo_it) {
      if (combo_it->second == combobox)
        return combo_it->first;
    }
  }

  return UNKNOWN_TYPE;
}
