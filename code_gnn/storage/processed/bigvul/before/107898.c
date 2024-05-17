void AfterTranslateInfoBar::RunMenu(views::View* source,
                                    const gfx::Point& pt) {
  if (source == original_language_menu_button_) {
    if (!original_language_menu_.get()) {
      original_language_menu_.reset(
          new views::Menu2(&original_language_menu_model_));
    }
    original_language_menu_->RunMenuAt(pt, views::Menu2::ALIGN_TOPRIGHT);
  } else if (source == target_language_menu_button_) {
      if (!target_language_menu_.get()) {
        target_language_menu_.reset(
            new views::Menu2(&target_language_menu_model_));
      }
      target_language_menu_->RunMenuAt(pt, views::Menu2::ALIGN_TOPRIGHT);
  } else if (source == options_menu_button_) {
    if (!options_menu_.get())
      options_menu_.reset(new views::Menu2(&options_menu_model_));
    options_menu_->RunMenuAt(pt, views::Menu2::ALIGN_TOPRIGHT);
  } else {
    NOTREACHED();
  }
}
