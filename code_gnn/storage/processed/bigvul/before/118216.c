void AutofillDialogViews::FocusInitialView() {
  views::View* to_focus = GetInitiallyFocusedView();
  if (to_focus && !to_focus->HasFocus())
    to_focus->RequestFocus();
}
