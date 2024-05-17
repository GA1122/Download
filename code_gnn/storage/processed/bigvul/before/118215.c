void AutofillDialogViews::FillSection(DialogSection section,
                                      ServerFieldType originating_type) {
  DetailsGroup* group = GroupForSection(section);
  TextfieldMap::iterator text_mapping =
      group->textfields.find(originating_type);
  if (text_mapping != group->textfields.end())
    text_mapping->second->SetText(base::string16());

  if (section == GetCreditCardSection() &&
      AutofillType(originating_type).group() == CREDIT_CARD) {
    for (ComboboxMap::const_iterator it = group->comboboxes.begin();
         it != group->comboboxes.end(); ++it) {
      if (AutofillType(it->first).group() == CREDIT_CARD)
        it->second->SetSelectedIndex(it->second->model()->GetDefaultIndex());
    }
  }

  UpdateSectionImpl(section, false);
}
