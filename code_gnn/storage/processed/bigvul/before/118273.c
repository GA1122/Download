void AutofillDialogViews::OnMenuButtonClicked(views::View* source,
                                              const gfx::Point& point) {
  DCHECK_EQ(kSuggestedButtonClassName, source->GetClassName());

  DetailsGroup* group = NULL;
  for (DetailGroupMap::iterator iter = detail_groups_.begin();
       iter != detail_groups_.end(); ++iter) {
    if (source == iter->second.suggested_button) {
      group = &iter->second;
      break;
    }
  }
  DCHECK(group);

  if (!group->suggested_button->visible())
    return;

  menu_runner_.reset(
      new views::MenuRunner(delegate_->MenuModelForSection(group->section), 0));

  group->container->SetActive(true);

  gfx::Rect screen_bounds = source->GetBoundsInScreen();
  screen_bounds.Inset(source->GetInsets());
  if (menu_runner_->RunMenuAt(source->GetWidget(),
                              group->suggested_button,
                              screen_bounds,
                              views::MENU_ANCHOR_TOPRIGHT,
                              ui::MENU_SOURCE_NONE) ==
      views::MenuRunner::MENU_DELETED) {
    return;
  }

  group->container->SetActive(false);
}
