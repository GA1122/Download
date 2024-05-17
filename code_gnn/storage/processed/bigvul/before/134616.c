bool OSExchangeDataProviderAura::HasCustomFormat(
    const OSExchangeData::CustomFormat& format) const {
  return pickle_data_.find(format) != pickle_data_.end();
}
