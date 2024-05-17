views::ImageButton* CreateBackButton(views::ButtonListener* listener) {
  views::ImageButton* back_button = new views::ImageButton(listener);
  back_button->SetImageAlignment(views::ImageButton::ALIGN_LEFT,
                                 views::ImageButton::ALIGN_MIDDLE);
  ui::ResourceBundle* rb = &ui::ResourceBundle::GetSharedInstance();
  back_button->SetImage(views::ImageButton::STATE_NORMAL,
                        rb->GetImageSkiaNamed(IDR_BACK));
  back_button->SetImage(views::ImageButton::STATE_HOVERED,
                        rb->GetImageSkiaNamed(IDR_BACK_H));
  back_button->SetImage(views::ImageButton::STATE_PRESSED,
                        rb->GetImageSkiaNamed(IDR_BACK_P));
  back_button->SetImage(views::ImageButton::STATE_DISABLED,
                        rb->GetImageSkiaNamed(IDR_BACK_D));
  back_button->SetFocusForPlatform();
  return back_button;
}
