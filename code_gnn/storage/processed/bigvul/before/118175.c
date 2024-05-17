SkColor DialogNotification::GetTextColor() const {
  switch (type_) {
    case DialogNotification::REQUIRED_ACTION:
    case DialogNotification::WALLET_ERROR:
    case DialogNotification::WALLET_USAGE_CONFIRMATION:
      return SkColorSetRGB(102, 102, 102);
    case DialogNotification::DEVELOPER_WARNING:
    case DialogNotification::SECURITY_WARNING:
      return SK_ColorWHITE;
    case DialogNotification::NONE:
      return SK_ColorTRANSPARENT;
  }

  NOTREACHED();
  return SK_ColorTRANSPARENT;
}
