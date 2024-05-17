OSExchangeData::Provider* OSExchangeDataProviderWin::Clone() const {
  return new OSExchangeDataProviderWin(data_object());
}
