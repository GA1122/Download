void ProfileChooserView::ShowBubble(
    profiles::BubbleViewMode view_mode,
    const signin::ManageAccountsParams& manage_accounts_params,
    signin_metrics::AccessPoint access_point,
    views::Button* anchor_button,
    gfx::NativeView parent_window,
    const gfx::Rect& anchor_rect,
    Browser* browser,
    bool is_source_keyboard) {
  if (IsShowing())
    return;

  profile_bubble_ =
      new ProfileChooserView(anchor_button, browser, view_mode,
                             manage_accounts_params.service_type, access_point);
  if (anchor_button) {
    anchor_button->AnimateInkDrop(views::InkDropState::ACTIVATED, nullptr);
  } else {
    DCHECK(parent_window);
    profile_bubble_->SetAnchorRect(anchor_rect);
    profile_bubble_->set_parent_window(parent_window);
  }

  views::Widget* widget =
      views::BubbleDialogDelegateView::CreateBubble(profile_bubble_);
  widget->Show();
  base::RecordAction(base::UserMetricsAction("ProfileChooser_Show"));
  if (is_source_keyboard)
    profile_bubble_->FocusFirstProfileButton();
}
