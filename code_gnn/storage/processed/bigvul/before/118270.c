void AutofillDialogViews::OnDidChangeFocus(
    views::View* focused_before,
    views::View* focused_now) {
  if (focused_before) {
    DetailsGroup* group = GroupForView(focused_before);
    if (group && group->container->visible())
      ValidateGroup(*group, VALIDATE_EDIT);
  }

  if (focused_now) {
    focused_now->ScrollRectToVisible(focused_now->GetLocalBounds());
    ShowErrorBubbleForViewIfNecessary(focused_now);
  }
}
