bool DialogNotification::HasArrow() const {
  return type_ == DialogNotification::WALLET_ERROR ||
         type_ == DialogNotification::WALLET_USAGE_CONFIRMATION;
}
