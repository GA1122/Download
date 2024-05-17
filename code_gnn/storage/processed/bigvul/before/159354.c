ChromeExtensionsAPIClient::GetFeedbackPrivateDelegate() {
  if (!feedback_private_delegate_) {
    feedback_private_delegate_ =
        base::MakeUnique<ChromeFeedbackPrivateDelegate>();
  }
  return feedback_private_delegate_.get();
}
