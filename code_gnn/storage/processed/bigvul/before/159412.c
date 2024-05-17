ExtensionsAPIClient::CreateGuestViewManagerDelegate(
    content::BrowserContext* context) const {
  return std::make_unique<ExtensionsGuestViewManagerDelegate>(context);
}
