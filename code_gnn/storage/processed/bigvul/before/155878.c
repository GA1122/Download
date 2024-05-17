views::View* ProfileChooserView::CreateDiceSyncErrorView(
    const AvatarMenu::Item& avatar_item,
    sync_ui_util::AvatarSyncErrorType error,
    int button_string_id) {
  views::View* view = new views::View();
  const int current_profile_vertical_margin =
      ChromeLayoutProvider::Get()->GetDistanceMetric(
          views::DISTANCE_CONTROL_VERTICAL_TEXT_PADDING);
  view->SetLayoutManager(std::make_unique<views::BoxLayout>(
      views::BoxLayout::kVertical,
      gfx::Insets(current_profile_vertical_margin, 0),
      current_profile_vertical_margin));

  const bool show_sync_paused_ui = error == sync_ui_util::AUTH_ERROR;
  const bool sync_disabled = !browser_->profile()->IsSyncAllowed();
  auto current_profile_photo = std::make_unique<BadgedProfilePhoto>(
      show_sync_paused_ui
          ? BadgedProfilePhoto::BADGE_TYPE_SYNC_PAUSED
          : sync_disabled ? BadgedProfilePhoto::BADGE_TYPE_SYNC_DISABLED
                          : BadgedProfilePhoto::BADGE_TYPE_SYNC_ERROR,
      avatar_item.icon);
  HoverButton* current_profile = new HoverButton(
      this, std::move(current_profile_photo),
      l10n_util::GetStringUTF16(
          show_sync_paused_ui
              ? IDS_PROFILES_DICE_SYNC_PAUSED_TITLE
              : sync_disabled ? IDS_PROFILES_DICE_SYNC_DISABLED_TITLE
                              : IDS_SYNC_ERROR_USER_MENU_TITLE),
      avatar_item.username);

  if (!show_sync_paused_ui && !sync_disabled) {
    current_profile->SetStyle(HoverButton::STYLE_ERROR);
    current_profile->SetEnabled(false);
  }

  view->AddChildView(current_profile);
  current_profile_card_ = current_profile;

  if (sync_disabled)
    return view;

  sync_error_button_ = views::MdTextButton::CreateSecondaryUiBlueButton(
      this, l10n_util::GetStringUTF16(button_string_id));
  sync_error_button_->set_id(error);
  base::RecordAction(
      base::UserMetricsAction("ProfileChooser_SignInAgainDisplayed"));
  views::View* padded_view = new views::View();
  padded_view->SetLayoutManager(std::make_unique<views::FillLayout>());
  padded_view->SetBorder(views::CreateEmptyBorder(
      0, kMenuEdgeMargin, kMenuEdgeMargin - current_profile_vertical_margin,
      kMenuEdgeMargin));
  padded_view->AddChildView(sync_error_button_);
  view->AddChildView(padded_view);
  return view;
}
