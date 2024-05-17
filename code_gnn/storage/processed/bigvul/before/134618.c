bool OSExchangeDataProviderAura::HasHtml() const {
  return ((formats_ & OSExchangeData::HTML) != 0);
}
