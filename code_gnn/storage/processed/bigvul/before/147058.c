WebURLRequest WebLocalFrameImpl::RequestFromHistoryItem(
    const WebHistoryItem& item,
    WebCachePolicy cache_policy) const {
  HistoryItem* history_item = item;
  return WrappedResourceRequest(
      history_item->GenerateResourceRequest(cache_policy));
}
