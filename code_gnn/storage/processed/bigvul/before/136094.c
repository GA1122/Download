void WebsiteSettingsPopupView::OnWidgetDestroying(views::Widget* widget) {
  is_popup_showing = false;
  presenter_->OnUIClosing();
}
