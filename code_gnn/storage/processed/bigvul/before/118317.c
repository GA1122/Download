void AutofillDialogViews::AccountChooser::Update() {
  SetVisible(delegate_->ShouldShowAccountChooser());

  gfx::Image icon = delegate_->AccountChooserImage();
  image_->SetImage(icon.AsImageSkia());
  menu_button_->SetText(delegate_->AccountChooserText());
  menu_button_->SetMinSize(gfx::Size());

  bool show_link = !delegate_->MenuModelForAccountChooser();
  menu_button_->SetVisible(!show_link);
  link_->SetText(delegate_->SignInLinkText());
  link_->SetVisible(show_link);

  menu_runner_.reset();

  PreferredSizeChanged();
}
