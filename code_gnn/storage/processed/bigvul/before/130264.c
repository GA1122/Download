IDataObjectAsyncCapability* OSExchangeDataProviderWin::GetIAsyncOperation(
    const OSExchangeData& data) {
  return static_cast<const OSExchangeDataProviderWin*>(&data.provider())->
      async_operation();
}
