bool OSExchangeDataProviderAura::HasURL() const {
  if ((formats_ & OSExchangeData::URL) != 0) {
    return true;
  }
  return GetPlainTextURL(NULL);
}
