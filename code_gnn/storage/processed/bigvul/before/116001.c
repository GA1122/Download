string16 ExtensionGlobalError::GetBubbleViewMessage() {
  if (message_.empty()) {
    message_ = GenerateMessage();
  }
  return message_;
}
