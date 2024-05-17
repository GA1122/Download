OSExchangeData::Provider* OSExchangeDataProviderAuraX11::Clone() const {
  OSExchangeDataProviderAuraX11* ret = new OSExchangeDataProviderAuraX11();
  ret->format_map_ = format_map_;
  return ret;
}
