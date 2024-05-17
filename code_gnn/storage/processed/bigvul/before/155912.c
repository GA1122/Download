void ProfileChooserView::ShowView(profiles::BubbleViewMode view_to_display,
                                  AvatarMenu* avatar_menu) {
  if (view_to_display == profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT) {
    DCHECK(AccountConsistencyModeManager::IsMirrorEnabledForProfile(
        browser_->profile()));
    const AvatarMenu::Item& active_item = avatar_menu->GetItemAt(
        avatar_menu->GetActiveProfileIndex());
    if (!active_item.signed_in) {
      view_to_display = profiles::BUBBLE_VIEW_MODE_PROFILE_CHOOSER;
    }
  }

  if (browser_->profile()->IsSupervised() &&
      (view_to_display == profiles::BUBBLE_VIEW_MODE_GAIA_ADD_ACCOUNT ||
       view_to_display == profiles::BUBBLE_VIEW_MODE_ACCOUNT_REMOVAL)) {
    LOG(WARNING) << "Supervised user attempted to add/remove account";
    return;
  }

  ResetView();
  RemoveAllChildViews(true);
  view_mode_ = view_to_display;

  views::GridLayout* layout = nullptr;
  views::View* sub_view = nullptr;
  views::View* view_to_focus = nullptr;
  switch (view_mode_) {
    case profiles::BUBBLE_VIEW_MODE_GAIA_SIGNIN:
    case profiles::BUBBLE_VIEW_MODE_GAIA_ADD_ACCOUNT:
    case profiles::BUBBLE_VIEW_MODE_GAIA_REAUTH:
      NOTREACHED();
      break;
    case profiles::BUBBLE_VIEW_MODE_ACCOUNT_REMOVAL:
      layout = CreateSingleColumnLayout(this, kFixedAccountRemovalViewWidth);
      sub_view = CreateAccountRemovalView();
      break;
    case profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT:
    case profiles::BUBBLE_VIEW_MODE_PROFILE_CHOOSER:
      layout = CreateSingleColumnLayout(this, menu_width_);
      sub_view = CreateProfileChooserView(avatar_menu);
      break;
  }

  views::ScrollView* scroll_view = new views::ScrollView;
  scroll_view->set_hide_horizontal_scrollbar(true);
  scroll_view->set_draw_overflow_indicator(false);
  scroll_view->ClipHeightTo(0, GetMaxHeight());
  scroll_view->SetContents(sub_view);

  layout->StartRow(1.0, 0);
  layout->AddView(scroll_view);
  if (GetBubbleFrameView()) {
    SizeToContents();
    Layout();
  }
  if (view_to_focus)
    view_to_focus->RequestFocus();
}
