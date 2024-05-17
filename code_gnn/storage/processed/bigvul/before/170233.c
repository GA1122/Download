extensions::MessagingDelegate::PolicyPermission IsNativeMessagingHostAllowed(
    content::BrowserContext* browser_context,
    const std::string& native_host_name) {
  extensions::MessagingDelegate* messaging_delegate =
      extensions::ExtensionsAPIClient::Get()->GetMessagingDelegate();
  EXPECT_NE(messaging_delegate, nullptr);
  return messaging_delegate->IsNativeMessagingHostAllowed(browser_context,
                                                          native_host_name);
}
