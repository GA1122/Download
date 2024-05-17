bool OSExchangeDataProviderAura::HasFile() const {
  return (formats_ & OSExchangeData::FILE_NAME) != 0;
}
