void AutofillDialogViews::InputEditedOrActivated(ServerFieldType type,
                                                 const gfx::Rect& bounds,
                                                 bool was_edit) {
  DCHECK_NE(UNKNOWN_TYPE, type);

  ExpandingTextfield* textfield = TextfieldForType(type);
  views::Combobox* combobox = ComboboxForType(type);

  if (!combobox && !textfield)
    return;

  DCHECK_NE(!!combobox, !!textfield);
  DetailsGroup* group = textfield ? GroupForView(textfield) :
                                    GroupForView(combobox);
  base::string16 text = textfield ?
      textfield->GetText() :
      combobox->model()->GetItemAt(combobox->selected_index());
  DCHECK(group);

  delegate_->UserEditedOrActivatedInput(group->section,
                                        type,
                                        GetWidget()->GetNativeView(),
                                        bounds,
                                        text,
                                        was_edit);

  if (textfield && textfield->invalid() && was_edit) {
    SetValidityForInput(
        textfield,
        delegate_->InputValidityMessage(
            group->section, type, textfield->GetText()));

    if (!textfield->invalid())
      ValidateGroup(*group, VALIDATE_EDIT);
  }

  if (delegate_->FieldControlsIcons(type))
    SetIconsForSection(group->section);

  SetEditabilityForSection(group->section);
}
