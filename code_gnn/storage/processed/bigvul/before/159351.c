ChromeExtensionsAPIClient::CreateWebRequestEventRouterDelegate() const {
  return base::MakeUnique<ChromeExtensionWebRequestEventRouterDelegate>();
}
