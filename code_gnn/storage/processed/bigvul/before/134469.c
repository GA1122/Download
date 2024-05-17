bool HomeButton::CanDrop(const OSExchangeData& data) {
  return data.HasURL();
}
