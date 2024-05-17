void AutofillDialogViews::UpdateSectionImpl(
    DialogSection section,
    bool clobber_inputs) {
  DetailsGroup* group = GroupForSection(section);

  if (clobber_inputs) {
    ServerFieldType type = UNKNOWN_TYPE;
    views::View* focused = GetFocusManager()->GetFocusedView();
    if (focused && group->container->Contains(focused)) {
      if (focused->GetClassName() == ExpandingTextfield::kViewClassName)
        type = TypeForTextfield(focused);
      else if (focused->GetClassName() == views::Combobox::kViewClassName)
        type = TypeForCombobox(static_cast<views::Combobox*>(focused));
    }

    InitInputsView(section);

    if (type != UNKNOWN_TYPE) {
      views::View* to_focus = TextfieldForType(type);
      if (!to_focus) to_focus = ComboboxForType(type);
      if (to_focus)
        to_focus->RequestFocus();
    }
  } else {
    const DetailInputs& updated_inputs =
        delegate_->RequestedFieldsForSection(section);

    for (DetailInputs::const_iterator iter = updated_inputs.begin();
         iter != updated_inputs.end(); ++iter) {
      const DetailInput& input = *iter;

      TextfieldMap::iterator text_mapping = group->textfields.find(input.type);
      if (text_mapping != group->textfields.end()) {
        ExpandingTextfield* textfield = text_mapping->second;
        if (textfield->GetText().empty())
          textfield->SetText(input.initial_value);
      }

      ComboboxMap::iterator combo_mapping = group->comboboxes.find(input.type);
      if (combo_mapping != group->comboboxes.end()) {
        views::Combobox* combobox = combo_mapping->second;
        if (combobox->selected_index() == combobox->model()->GetDefaultIndex())
          SelectComboboxValueOrSetToDefault(combobox, input.initial_value);
      }
    }

    SetIconsForSection(section);
  }

  SetEditabilityForSection(section);
  UpdateDetailsGroupState(*group);
}
