void OSExchangeDataProviderAura::SetString(const base::string16& data) {
  string_ = data;
  formats_ |= OSExchangeData::STRING;
}
