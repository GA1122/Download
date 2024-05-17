ui::OSExchangeData::Provider* CloneProvider(const ui::OSExchangeData& data) {
  return data.provider().Clone();
}
