  virtual void ButtonPressed(views::Button* sender, const views::Event& event) {
    if (sender == close_button_)
      bubble_->StartFade(false);
    else
      NOTREACHED() << "Unknown view";
  }
