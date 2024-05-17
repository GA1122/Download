int64_t ExtractPostId(const WebHistoryItem& item) {
  if (item.IsNull() || item.HttpBody().IsNull())
    return -1;

  return item.HttpBody().Identifier();
}
