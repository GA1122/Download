void AutofillDialogViews::SetEditabilityForSection(DialogSection section) {
  const DetailInputs& inputs =
      delegate_->RequestedFieldsForSection(section);
  DetailsGroup* group = GroupForSection(section);

  for (DetailInputs::const_iterator iter = inputs.begin();
       iter != inputs.end(); ++iter) {
    const DetailInput& input = *iter;
    bool editable = delegate_->InputIsEditable(input, section);

    TextfieldMap::iterator text_mapping = group->textfields.find(input.type);
    if (text_mapping != group->textfields.end()) {
      ExpandingTextfield* textfield = text_mapping->second;
      textfield->SetEditable(editable);
      continue;
    }

    ComboboxMap::iterator combo_mapping = group->comboboxes.find(input.type);
    if (combo_mapping != group->comboboxes.end()) {
      views::Combobox* combobox = combo_mapping->second;
      combobox->SetEnabled(editable);
    }
  }
}
