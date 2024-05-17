  TitleCard(const base::string16& message, views::ButtonListener* listener,
            views::ImageButton** back_button) {
    back_button_ = CreateBackButton(listener);
    *back_button = back_button_;

    title_label_ =
        new views::Label(message, views::style::CONTEXT_DIALOG_TITLE);
    title_label_->SetHorizontalAlignment(gfx::ALIGN_CENTER);

    AddChildView(back_button_);
    AddChildView(title_label_);
  }
