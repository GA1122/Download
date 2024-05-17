int64 ExtractPostId(const WebHistoryItem& item) {
  if (item.isNull())
    return -1;

  if (item.httpBody().isNull())
    return -1;

  return item.httpBody().identifier();
}
