void AutofillDialogViews::AccountChooser::OnMenuButtonClicked(
    views::View* source,
    const gfx::Point& point) {
  DCHECK_EQ(menu_button_, source);

  ui::MenuModel* model = delegate_->MenuModelForAccountChooser();
  if (!model)
    return;

  menu_runner_.reset(new views::MenuRunner(model, 0));
  if (menu_runner_->RunMenuAt(source->GetWidget(),
                              NULL,
                              source->GetBoundsInScreen(),
                              views::MENU_ANCHOR_TOPRIGHT,
                              ui::MENU_SOURCE_NONE) ==
      views::MenuRunner::MENU_DELETED) {
    return;
  }
}
