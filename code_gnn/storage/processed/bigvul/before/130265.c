IDataObject* OSExchangeDataProviderWin::GetIDataObject(
    const OSExchangeData& data) {
  return static_cast<const OSExchangeDataProviderWin*>(&data.provider())->
      data_object();
}
