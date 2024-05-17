void AutofillDialogViews::InitChildViews() {
  button_strip_extra_view_ = new LayoutPropagationView();
  button_strip_extra_view_->SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kHorizontal, 0, 0, 0));

  save_in_chrome_checkbox_container_ = new views::View();
  save_in_chrome_checkbox_container_->SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kHorizontal, 0, 0, 7));
  button_strip_extra_view_->AddChildView(save_in_chrome_checkbox_container_);

  save_in_chrome_checkbox_ =
      new views::Checkbox(delegate_->SaveLocallyText());
  save_in_chrome_checkbox_->SetChecked(delegate_->ShouldSaveInChrome());
  save_in_chrome_checkbox_container_->AddChildView(save_in_chrome_checkbox_);

  save_in_chrome_checkbox_container_->AddChildView(
      new TooltipIcon(delegate_->SaveLocallyTooltip()));

  button_strip_image_ = new views::ImageView();
  button_strip_extra_view_->AddChildView(button_strip_image_);

  account_chooser_ = new AccountChooser(delegate_);
  notification_area_ = new NotificationArea(delegate_);
  notification_area_->set_arrow_centering_anchor(account_chooser_->AsWeakPtr());
  AddChildView(notification_area_);

  scrollable_area_ = new views::ScrollView();
  scrollable_area_->set_hide_horizontal_scrollbar(true);
  scrollable_area_->SetContents(CreateDetailsContainer());
  AddChildView(scrollable_area_);

  loading_shield_ = new LoadingAnimationView(delegate_->SpinnerText());
  AddChildView(loading_shield_);

  sign_in_web_view_ = new views::WebView(delegate_->profile());
  AddChildView(sign_in_web_view_);

  overlay_view_ = new OverlayView(delegate_);
  overlay_view_->SetVisible(false);
}
