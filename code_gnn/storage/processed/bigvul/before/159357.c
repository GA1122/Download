MessagingDelegate* ChromeExtensionsAPIClient::GetMessagingDelegate() {
  if (!messaging_delegate_)
    messaging_delegate_ = base::MakeUnique<ChromeMessagingDelegate>();
  return messaging_delegate_.get();
}
