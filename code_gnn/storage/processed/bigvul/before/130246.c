bool OSExchangeDataProviderWin::DidOriginateFromRenderer() const {
  return HasCustomFormat(GetRendererTaintCustomType());
}
