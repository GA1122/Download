AutofillDialogViews::AccountChooser::AccountChooser(
    AutofillDialogViewDelegate* delegate)
    : image_(new views::ImageView()),
      menu_button_(new views::MenuButton(NULL, base::string16(), this, true)),
      link_(new views::Link()),
      delegate_(delegate) {
  SetBorder(views::Border::CreateEmptyBorder(0, 0, 0, 10));
  SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kHorizontal, 0, 0,
                           kAroundTextPadding));
  AddChildView(image_);

  menu_button_->set_background(NULL);
  menu_button_->SetBorder(views::Border::NullBorder());
  gfx::Insets insets = GetInsets();
  menu_button_->SetFocusPainter(
      views::Painter::CreateDashedFocusPainterWithInsets(insets));
  menu_button_->SetFocusable(true);
  AddChildView(menu_button_);

  link_->set_listener(this);
  AddChildView(link_);
}
