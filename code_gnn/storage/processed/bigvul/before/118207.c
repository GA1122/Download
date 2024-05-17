views::View* AutofillDialogViews::CreateInputsContainer(DialogSection section) {
  views::View* info_view = new views::View();
  info_view->SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 0));

  DetailsGroup* group = GroupForSection(section);
  group->manual_input = new views::View();
  InitInputsView(section);
  info_view->AddChildView(group->manual_input);

  group->suggested_info = new SuggestionView(this);
  info_view->AddChildView(group->suggested_info);

  group->suggested_button = new SuggestedButton(this);

  return info_view;
}
