void AutofillDialogViews::CreateDetailsSection(DialogSection section) {
  views::View* inputs_container = CreateInputsContainer(section);

  DetailsGroup* group = GroupForSection(section);
  group->container = new SectionContainer(delegate_->LabelForSection(section),
                                          inputs_container,
                                          group->suggested_button);
  DCHECK(group->suggested_button->parent());
  UpdateDetailsGroupState(*group);
}
