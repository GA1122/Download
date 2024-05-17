void WebsiteSettingsPopupView::ButtonPressed(views::Button* button,
                                             const ui::Event& event) {
  if (button == reset_decisions_button_)
    presenter_->OnRevokeSSLErrorBypassButtonPressed();
  GetWidget()->Close();
}
