views::View* ProfileChooserView::CreateCurrentProfileAccountsView(
    const AvatarMenu::Item& avatar_item) {
  DCHECK(avatar_item.signed_in);
  views::View* view = new views::View();
  view->SetBackground(views::CreateSolidBackground(
      profiles::kAvatarBubbleAccountsBackgroundColor));
  views::GridLayout* layout = CreateSingleColumnLayout(view, menu_width_);

  Profile* profile = browser_->profile();
  std::string error_account_id = GetAuthErrorAccountId(profile);

  auto* identity_manager = IdentityManagerFactory::GetForProfile(profile);
  DCHECK(identity_manager->HasPrimaryAccount());
  AccountInfo primary_account = identity_manager->GetPrimaryAccountInfo();

  CreateAccountButton(layout, primary_account.account_id, true,
                      error_account_id == primary_account.account_id,
                      menu_width_);
  for (const AccountInfo& account :
       profiles::GetSecondaryAccountsForSignedInProfile(profile))
    CreateAccountButton(layout, account.account_id, false,
                        error_account_id == account.account_id, menu_width_);

  ChromeLayoutProvider* provider = ChromeLayoutProvider::Get();
  const int vertical_spacing =
      provider->GetDistanceMetric(views::DISTANCE_RELATED_CONTROL_VERTICAL);
  if (!profile->IsSupervised()) {
    layout->AddPaddingRow(views::GridLayout::kFixedSize, vertical_spacing);

    add_account_link_ = CreateLink(l10n_util::GetStringFUTF16(
        IDS_PROFILES_PROFILE_ADD_ACCOUNT_BUTTON, avatar_item.name), this);
    add_account_link_->SetBorder(views::CreateEmptyBorder(
        0, provider->GetInsetsMetric(views::INSETS_DIALOG).left(),
        vertical_spacing, 0));
    layout->StartRow(1.0, 0);
    layout->AddView(add_account_link_);
  }

  return view;
}
