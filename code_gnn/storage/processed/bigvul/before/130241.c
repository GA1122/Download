OSExchangeData::Provider* OSExchangeData::CreateProvider() {
  return new OSExchangeDataProviderWin();
}
