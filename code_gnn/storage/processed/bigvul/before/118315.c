ExpandingTextfield* AutofillDialogViews::TextfieldForType(
    ServerFieldType type) {
  if (type == CREDIT_CARD_VERIFICATION_CODE) {
    DetailsGroup* group = GroupForSection(GetCreditCardSection());
    if (!group->manual_input->visible())
      return group->suggested_info->textfield();
  }

  for (DetailGroupMap::iterator iter = detail_groups_.begin();
       iter != detail_groups_.end(); ++iter) {
    const DetailsGroup& group = iter->second;
    if (!delegate_->SectionIsActive(group.section))
      continue;

    TextfieldMap::const_iterator text_mapping = group.textfields.find(type);
    if (text_mapping != group.textfields.end())
      return text_mapping->second;
  }

  return NULL;
}
