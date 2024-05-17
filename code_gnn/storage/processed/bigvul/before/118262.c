void AutofillDialogViews::MarkInputsInvalid(
    DialogSection section,
    const ValidityMessages& messages,
    bool overwrite_unsure) {
  DetailsGroup* group = GroupForSection(section);
  DCHECK(group->container->visible());

  if (group->manual_input->visible()) {
    for (TextfieldMap::const_iterator iter = group->textfields.begin();
         iter != group->textfields.end(); ++iter) {
      const ValidityMessage& message =
          messages.GetMessageOrDefault(iter->first);
      if (overwrite_unsure || message.sure)
        SetValidityForInput(iter->second, message.text);
    }
    for (ComboboxMap::const_iterator iter = group->comboboxes.begin();
         iter != group->comboboxes.end(); ++iter) {
      const ValidityMessage& message =
          messages.GetMessageOrDefault(iter->first);
      if (overwrite_unsure || message.sure)
        SetValidityForInput(iter->second, message.text);
    }
  } else {
    EraseInvalidViewsInGroup(group);

    if (section == GetCreditCardSection()) {
      const ValidityMessage& message =
          messages.GetMessageOrDefault(CREDIT_CARD_VERIFICATION_CODE);
      if (overwrite_unsure || message.sure) {
        SetValidityForInput(group->suggested_info->textfield(), message.text);
      }
    }
  }
}
