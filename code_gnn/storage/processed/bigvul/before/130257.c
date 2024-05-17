DataObjectImpl* OSExchangeDataProviderWin::GetDataObjectImpl(
    const OSExchangeData& data) {
  return static_cast<const OSExchangeDataProviderWin*>(&data.provider())->
      data_.get();
}
