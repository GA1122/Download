int64 ExtractPostId(HistoryEntry* entry) {
  if (!entry)
    return -1;

  const WebHistoryItem& item = entry->root();
  if (item.isNull() || item.httpBody().isNull())
    return -1;

  return item.httpBody().identifier();
}
