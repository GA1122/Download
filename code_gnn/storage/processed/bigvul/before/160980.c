PagePopup* ChromeClientImpl::OpenPagePopup(PagePopupClient* client) {
  return web_view_->OpenPagePopup(client);
}
