void AutofillDialogViews::GetUserInput(DialogSection section,
                                       FieldValueMap* output) {
  DetailsGroup* group = GroupForSection(section);
  for (TextfieldMap::const_iterator it = group->textfields.begin();
       it != group->textfields.end(); ++it) {
    output->insert(std::make_pair(it->first, it->second->GetText()));
  }
  for (ComboboxMap::const_iterator it = group->comboboxes.begin();
       it != group->comboboxes.end(); ++it) {
    output->insert(std::make_pair(it->first,
        it->second->model()->GetItemAt(it->second->selected_index())));
  }
}
