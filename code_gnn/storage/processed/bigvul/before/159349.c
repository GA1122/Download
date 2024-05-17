ChromeExtensionsAPIClient::CreateMimeHandlerViewGuestDelegate(
    MimeHandlerViewGuest* guest) const {
  return base::MakeUnique<ChromeMimeHandlerViewGuestDelegate>();
}
