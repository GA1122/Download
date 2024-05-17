void BubbleHeaderView::AddPasswordReuseButtons() {
  change_password_button_ = views::MdTextButton::CreateSecondaryUiBlueButton(
      button_listener_,
      l10n_util::GetStringUTF16(IDS_PAGE_INFO_CHANGE_PASSWORD_BUTTON));
  change_password_button_->set_id(
      PageInfoBubbleView::VIEW_ID_PAGE_INFO_BUTTON_CHANGE_PASSWORD);
  whitelist_password_reuse_button_ =
      views::MdTextButton::CreateSecondaryUiButton(
          button_listener_, l10n_util::GetStringUTF16(
                                IDS_PAGE_INFO_WHITELIST_PASSWORD_REUSE_BUTTON));
  whitelist_password_reuse_button_->set_id(
      PageInfoBubbleView::VIEW_ID_PAGE_INFO_BUTTON_WHITELIST_PASSWORD_REUSE);

  int kSpacingBetweenButtons = 8;

  bool can_fit_in_one_line =
      (password_reuse_button_container_->width() - kSpacingBetweenButtons) >=
      (change_password_button_->CalculatePreferredSize().width() +
       whitelist_password_reuse_button_->CalculatePreferredSize().width());
  views::BoxLayout* layout =
      new views::BoxLayout(can_fit_in_one_line ? views::BoxLayout::kHorizontal
                                               : views::BoxLayout::kVertical,
                           gfx::Insets(), kSpacingBetweenButtons);
  layout->set_main_axis_alignment(views::BoxLayout::MAIN_AXIS_ALIGNMENT_START);
  password_reuse_button_container_->SetLayoutManager(layout);

#if defined(OS_WIN) || defined(OS_CHROMEOS)
  password_reuse_button_container_->AddChildView(change_password_button_);
  password_reuse_button_container_->AddChildView(
      whitelist_password_reuse_button_);
#else
  password_reuse_button_container_->AddChildView(
      whitelist_password_reuse_button_);
  password_reuse_button_container_->AddChildView(change_password_button_);
#endif

  password_reuse_button_container_->SetBorder(
      views::CreateEmptyBorder(8, 0, 0, 0));

  InvalidateLayout();
}
