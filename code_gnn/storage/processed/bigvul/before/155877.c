views::View* ProfileChooserView::CreateDiceSigninView() {
  IncrementDiceSigninPromoShowCount();
  dice_sync_promo_accounts_ =
      signin_ui_util::GetAccountsForDicePromos(browser_->profile());

  int promotext_top_spacing = 16;
  const int additional_bottom_spacing =
      dice_sync_promo_accounts_.empty() ? 0 : 8;
  views::View* view = new views::View();
  view->SetLayoutManager(std::make_unique<views::BoxLayout>(
      views::BoxLayout::kVertical,
      gfx::Insets(0, 0, additional_bottom_spacing, 0)));

  const bool promo_account_available = !dice_sync_promo_accounts_.empty();

  signin_metrics::RecordSigninImpressionUserActionForAccessPoint(
      signin_metrics::AccessPoint::ACCESS_POINT_AVATAR_BUBBLE_SIGN_IN);
  signin_metrics::RecordSigninImpressionWithAccountUserActionForAccessPoint(
      signin_metrics::AccessPoint::ACCESS_POINT_AVATAR_BUBBLE_SIGN_IN,
      promo_account_available);

  if (!promo_account_available) {
    if (GetDiceSigninPromoShowCount() <=
        kDiceSigninPromoIllustrationShowCountMax) {
      ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
      views::ImageView* illustration = new NonAccessibleImageView();
      illustration->SetImage(
          *rb.GetNativeImageNamed(IDR_PROFILES_DICE_TURN_ON_SYNC)
               .ToImageSkia());
      view->AddChildView(illustration);
      promotext_top_spacing = 24;
    }
    views::Label* promo = new views::Label(
        l10n_util::GetStringUTF16(IDS_PROFILES_DICE_SYNC_PROMO));
    promo->SetMultiLine(true);
    promo->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    promo->SetMaximumWidth(menu_width_ - 2 * kMenuEdgeMargin);
    promo->SetBorder(views::CreateEmptyBorder(
        promotext_top_spacing, kMenuEdgeMargin, 0, kMenuEdgeMargin));
    view->AddChildView(promo);

    dice_signin_button_view_ = new DiceSigninButtonView(this);
    dice_signin_button_view_->SetBorder(
        views::CreateEmptyBorder(gfx::Insets(kMenuEdgeMargin)));
    view->AddChildView(dice_signin_button_view_);
    signin_current_profile_button_ = dice_signin_button_view_->signin_button();
    return view;
  }
  AccountInfo dice_promo_default_account = dice_sync_promo_accounts_[0];
  gfx::Image account_icon =
      AccountTrackerServiceFactory::GetForProfile(browser_->profile())
          ->GetAccountImage(dice_promo_default_account.account_id);
  if (account_icon.IsEmpty()) {
    account_icon = ui::ResourceBundle::GetSharedInstance().GetImageNamed(
        profiles::GetPlaceholderAvatarIconResourceID());
  }
  dice_signin_button_view_ =
      new DiceSigninButtonView(dice_promo_default_account, account_icon, this,
                                false);
  signin_with_gaia_account_button_ = dice_signin_button_view_->signin_button();

  views::View* promo_button_container = new views::View();
  const int content_list_vert_spacing =
      ChromeLayoutProvider::Get()->GetDistanceMetric(
          DISTANCE_CONTENT_LIST_VERTICAL_MULTI);
  const int bottom_spacing = kMenuEdgeMargin - content_list_vert_spacing;
  promo_button_container->SetLayoutManager(std::make_unique<views::BoxLayout>(
      views::BoxLayout::kVertical,
      gfx::Insets(kMenuEdgeMargin, kMenuEdgeMargin, bottom_spacing,
                  kMenuEdgeMargin),
      content_list_vert_spacing));
  promo_button_container->AddChildView(dice_signin_button_view_);

  signout_button_ = views::MdTextButton::Create(
      this, l10n_util::GetStringUTF16(IDS_SCREEN_LOCK_SIGN_OUT),
      views::style::CONTEXT_BUTTON);
  promo_button_container->AddChildView(signout_button_);

  view->AddChildView(promo_button_container);

  return view;
}
