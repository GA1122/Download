SkColor DialogNotification::GetBorderColor() const {
  switch (type_) {
    case DialogNotification::WALLET_USAGE_CONFIRMATION:
      return SkColorSetRGB(0xe5, 0xe5, 0xe5);
    case DialogNotification::REQUIRED_ACTION:
    case DialogNotification::WALLET_ERROR:
    case DialogNotification::DEVELOPER_WARNING:
    case DialogNotification::SECURITY_WARNING:
    case DialogNotification::NONE:
      return GetBackgroundColor();
  }

  NOTREACHED();
  return SK_ColorTRANSPARENT;
}
