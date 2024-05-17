views::View* ProfileChooserView::CreateProfileChooserView(
    AvatarMenu* avatar_menu) {
  views::View* view = new views::View();
  views::GridLayout* layout = CreateSingleColumnLayout(view, menu_width_);
  Indexes other_profiles;
  views::View* sync_error_view = nullptr;
  views::View* current_profile_view = nullptr;
  views::View* current_profile_accounts = nullptr;
  views::View* option_buttons_view = nullptr;
  views::View* autofill_home_view = nullptr;
  bool current_profile_signed_in = false;
  for (size_t i = 0; i < avatar_menu->GetNumberOfItems(); ++i) {
    const AvatarMenu::Item& item = avatar_menu->GetItemAt(i);
    if (item.active) {
      option_buttons_view = CreateOptionsView(
          item.signed_in && profiles::IsLockAvailable(browser_->profile()),
          avatar_menu);
      current_profile_view = CreateCurrentProfileView(item, false);
      autofill_home_view = CreateAutofillHomeView();
      current_profile_signed_in = item.signed_in;
      if (!IsProfileChooser(view_mode_))
        current_profile_accounts = CreateCurrentProfileAccountsView(item);
      sync_error_view = CreateSyncErrorViewIfNeeded(item);
    } else {
      other_profiles.push_back(i);
    }
  }

  if (sync_error_view) {
    layout->StartRow(1.0, 0);
    layout->AddView(sync_error_view);
    layout->StartRow(views::GridLayout::kFixedSize, 0);
    layout->AddView(new views::Separator());
  }

  if (!current_profile_view) {
    current_profile_view = CreateGuestProfileView();
    option_buttons_view = CreateOptionsView(false, avatar_menu);
  }

  if (!(dice_enabled_ && sync_error_view)) {
    layout->StartRow(1.0, 0);
    layout->AddView(current_profile_view);
  }

  if (!IsProfileChooser(view_mode_)) {
    DCHECK(current_profile_accounts);
    layout->StartRow(views::GridLayout::kFixedSize, 0);
    layout->AddView(new views::Separator());
    layout->StartRow(1.0, 0);
    layout->AddView(current_profile_accounts);
  }

  if (browser_->profile()->IsSupervised()) {
    layout->StartRow(1.0, 0);
    layout->AddView(CreateSupervisedUserDisclaimerView());
  }

  if (autofill_home_view) {
    const int content_list_vert_spacing =
        ChromeLayoutProvider::Get()->GetDistanceMetric(
            DISTANCE_CONTENT_LIST_VERTICAL_MULTI);
    if (!current_profile_signed_in) {
      layout->StartRow(0, 0);
      layout->AddView(new views::Separator());
      layout->AddPaddingRow(1.0, content_list_vert_spacing);
    }
    layout->StartRow(0, 0);
    layout->AddView(autofill_home_view);
    layout->AddPaddingRow(1.0, content_list_vert_spacing);
  }

  layout->StartRow(views::GridLayout::kFixedSize, 0);
  layout->AddView(new views::Separator());

  if (option_buttons_view) {
    layout->StartRow(views::GridLayout::kFixedSize, 0);
    layout->AddView(option_buttons_view);
  }
  return view;
}
