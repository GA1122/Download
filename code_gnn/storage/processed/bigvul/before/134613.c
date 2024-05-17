bool OSExchangeDataProviderAura::GetPickledData(
    const OSExchangeData::CustomFormat& format,
    Pickle* data) const {
  PickleData::const_iterator i = pickle_data_.find(format);
  if (i == pickle_data_.end())
    return false;

  *data = i->second;
  return true;
}
