views::View* ProfileChooserView::CreateSyncErrorViewIfNeeded(
    const AvatarMenu::Item& avatar_item) {
  int content_string_id, button_string_id;
  SigninManagerBase* signin_manager =
      SigninManagerFactory::GetForProfile(browser_->profile());
  sync_ui_util::AvatarSyncErrorType error =
      sync_ui_util::GetMessagesForAvatarSyncError(
          browser_->profile(), *signin_manager, &content_string_id,
          &button_string_id);
  if (error == sync_ui_util::NO_SYNC_ERROR)
    return nullptr;

  ChromeLayoutProvider* provider = ChromeLayoutProvider::Get();

  if (error != sync_ui_util::SUPERVISED_USER_AUTH_ERROR && dice_enabled_)
    return CreateDiceSyncErrorView(avatar_item, error, button_string_id);

  views::View* view = new views::View();
  auto layout = std::make_unique<views::BoxLayout>(
      views::BoxLayout::kHorizontal, gfx::Insets(kMenuEdgeMargin),
      provider->GetDistanceMetric(DISTANCE_UNRELATED_CONTROL_HORIZONTAL));
  layout->set_cross_axis_alignment(
      views::BoxLayout::CROSS_AXIS_ALIGNMENT_START);
  view->SetLayoutManager(std::move(layout));

  views::ImageView* sync_problem_icon = new views::ImageView();
  sync_problem_icon->SetImage(
      gfx::CreateVectorIcon(kSyncProblemIcon, kIconSize, gfx::kGoogleRed700));
  view->AddChildView(sync_problem_icon);

  views::View* vertical_view = new views::View();
  const int small_vertical_spacing =
      provider->GetDistanceMetric(DISTANCE_RELATED_CONTROL_VERTICAL_SMALL);
  auto vertical_layout = std::make_unique<views::BoxLayout>(
      views::BoxLayout::kVertical, gfx::Insets(), small_vertical_spacing);
  vertical_layout->set_cross_axis_alignment(
      views::BoxLayout::CROSS_AXIS_ALIGNMENT_START);
  vertical_view->SetLayoutManager(std::move(vertical_layout));

  views::Label* title_label = new views::Label(
      l10n_util::GetStringUTF16(IDS_SYNC_ERROR_USER_MENU_TITLE));
  title_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  title_label->SetEnabledColor(gfx::kGoogleRed700);
  vertical_view->AddChildView(title_label);

  views::Label* content_label =
      new views::Label(l10n_util::GetStringUTF16(content_string_id));
  content_label->SetMultiLine(true);
  content_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  vertical_view->AddChildView(content_label);

  if (button_string_id) {
    auto* padding = new views::View;
    padding->SetPreferredSize(gfx::Size(
        0,
        provider->GetDistanceMetric(views::DISTANCE_RELATED_CONTROL_VERTICAL)));
    vertical_view->AddChildView(padding);

    sync_error_button_ = views::MdTextButton::CreateSecondaryUiBlueButton(
        this, l10n_util::GetStringUTF16(button_string_id));
    sync_error_button_->set_id(error);
    vertical_view->AddChildView(sync_error_button_);
    view->SetBorder(views::CreateEmptyBorder(0, 0, small_vertical_spacing, 0));
  }

  view->AddChildView(vertical_view);
  return view;
}
