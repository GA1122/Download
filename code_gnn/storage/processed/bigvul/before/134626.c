void OSExchangeDataProviderAura::SetPickledData(
    const OSExchangeData::CustomFormat& format,
    const Pickle& data) {
  pickle_data_[format] = data;
  formats_ |= OSExchangeData::PICKLED_DATA;
}
