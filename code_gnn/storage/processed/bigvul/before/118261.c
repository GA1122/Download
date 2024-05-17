  explicit LoadingAnimationView(const base::string16& text) :
      container_(new views::View()) {
    AddChildView(container_);
    container_->SetLayoutManager(
        new views::BoxLayout(views::BoxLayout::kHorizontal, 0, 0, 0));

    const gfx::FontList& font_list =
        ui::ResourceBundle::GetSharedInstance().GetFontList(
            ui::ResourceBundle::LargeFont);
    animation_.reset(new LoadingAnimation(this, font_list.GetHeight()));

    container_->AddChildView(new views::Label(text, font_list));

    for (size_t i = 0; i < 3; ++i) {
      container_->AddChildView(
          new views::Label(base::ASCIIToUTF16("."), font_list));
    }
  }
