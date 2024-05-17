void LockScreenMediaControlsView::CreateMediaButton(
    int size,
    MediaSessionAction action,
    const base::string16& accessible_name) {
  auto button = views::CreateVectorImageButton(this);
  button->set_tag(static_cast<int>(action));
  button->SetPreferredSize(kMediaButtonSize);
  button->SetAccessibleName(accessible_name);
  button->SetFocusBehavior(views::View::FocusBehavior::ALWAYS);

  views::SetImageFromVectorIcon(button.get(),
                                GetVectorIconForMediaAction(action), size,
                                kMediaButtonColor);

  button_row_->AddChildView(std::move(button));
}
