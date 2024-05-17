views::View* ProfileChooserView::CreateCurrentProfileView(
    const AvatarMenu::Item& avatar_item,
    bool is_guest) {
  Profile* profile = browser_->profile();
  const bool sync_disabled = !profile->IsSyncAllowed();
  if (!is_guest && sync_disabled)
    return CreateDiceSyncErrorView(avatar_item, sync_ui_util::NO_SYNC_ERROR, 0);

  if (!avatar_item.signed_in && dice_enabled_ &&
      SyncPromoUI::ShouldShowSyncPromo(profile)) {
    return CreateDiceSigninView();
  }

  ChromeLayoutProvider* provider = ChromeLayoutProvider::Get();

  views::View* view = new views::View();
  bool mirror_enabled =
      AccountConsistencyModeManager::IsMirrorEnabledForProfile(profile);
  int content_list_vert_spacing =
      mirror_enabled
          ? provider->GetDistanceMetric(DISTANCE_CONTENT_LIST_VERTICAL_MULTI)
          : provider->GetDistanceMetric(DISTANCE_CONTENT_LIST_VERTICAL_SINGLE);
  view->SetLayoutManager(std::make_unique<views::BoxLayout>(
      views::BoxLayout::kVertical, gfx::Insets(content_list_vert_spacing, 0),
      0));

  auto current_profile_photo = std::make_unique<BadgedProfilePhoto>(
      GetProfileBadgeType(profile), avatar_item.icon);
  const base::string16 profile_name =
      profiles::GetAvatarNameForProfile(profile->GetPath());

  bool show_email = !is_guest && avatar_item.signed_in && !mirror_enabled;
  const base::string16 hover_button_title =
      dice_enabled_ && profile->IsSyncAllowed() && show_email
          ? l10n_util::GetStringUTF16(IDS_PROFILES_SYNC_COMPLETE_TITLE)
          : profile_name;
  HoverButton* profile_card = new HoverButton(
      this, std::move(current_profile_photo), hover_button_title,
      show_email ? avatar_item.username : base::string16());
  if (show_email && !avatar_item.username.empty())
    profile_card->SetSubtitleElideBehavior(gfx::ELIDE_EMAIL);
  current_profile_card_ = profile_card;
  view->AddChildView(current_profile_card_);

  if (is_guest) {
    current_profile_card_->SetEnabled(false);
    return view;
  }

  if (avatar_item.signed_in) {
    if (mirror_enabled) {
      base::string16 button_text = l10n_util::GetStringUTF16(
          IsProfileChooser(view_mode_)
              ? IDS_PROFILES_PROFILE_MANAGE_ACCOUNTS_BUTTON
              : IDS_PROFILES_PROFILE_HIDE_MANAGE_ACCOUNTS_BUTTON);
      manage_accounts_button_ = new HoverButton(this, button_text);
      view->AddChildView(manage_accounts_button_);
    }

    current_profile_card_->SetAccessibleName(
        l10n_util::GetStringFUTF16(
            IDS_PROFILES_EDIT_SIGNED_IN_PROFILE_ACCESSIBLE_NAME,
            profile_name,
            avatar_item.username));
    return view;
  }

  if (!dice_enabled_ &&
      SigninManagerFactory::GetForProfile(profile)->IsSigninAllowed()) {
    views::View* extra_links_view = new views::View();
    extra_links_view->SetLayoutManager(std::make_unique<views::BoxLayout>(
        views::BoxLayout::kVertical,
        gfx::Insets(provider->GetDistanceMetric(
                        views::DISTANCE_RELATED_CONTROL_VERTICAL),
                    kMenuEdgeMargin),
        kMenuEdgeMargin));
    views::Label* promo =
        new views::Label(l10n_util::GetStringUTF16(IDS_PROFILES_SIGNIN_PROMO));
    promo->SetMultiLine(true);
    promo->SetHorizontalAlignment(gfx::ALIGN_LEFT);

    promo->SetMaximumWidth(menu_width_);
    extra_links_view->AddChildView(promo);

    signin_current_profile_button_ =
        views::MdTextButton::CreateSecondaryUiBlueButton(
            this, l10n_util::GetStringFUTF16(
                      IDS_SYNC_START_SYNC_BUTTON_LABEL,
                      l10n_util::GetStringUTF16(IDS_SHORT_PRODUCT_NAME)));
    extra_links_view->AddChildView(signin_current_profile_button_);
    signin_metrics::RecordSigninImpressionUserActionForAccessPoint(
        signin_metrics::AccessPoint::ACCESS_POINT_AVATAR_BUBBLE_SIGN_IN);
    extra_links_view->SetBorder(views::CreateEmptyBorder(
        0, 0,
        provider->GetDistanceMetric(DISTANCE_RELATED_CONTROL_VERTICAL_SMALL),
        0));
    view->AddChildView(extra_links_view);
  }

  current_profile_card_->SetAccessibleName(
      l10n_util::GetStringFUTF16(
          IDS_PROFILES_EDIT_PROFILE_ACCESSIBLE_NAME, profile_name));
  return view;
}
