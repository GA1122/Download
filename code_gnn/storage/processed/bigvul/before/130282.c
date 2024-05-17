OSExchangeDataProviderWin::OSExchangeDataProviderWin(IDataObject* source)
    : data_(new DataObjectImpl()),
      source_object_(source) {
}
