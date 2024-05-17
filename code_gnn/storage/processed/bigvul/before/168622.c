HistogramIDBKeyPathType HistogramKeyPathType(const IndexedDBKeyPath& key_path) {
  switch (key_path.type()) {
    case blink::kWebIDBKeyPathTypeNull:
      return KEY_PATH_TYPE_NONE;
    case blink::kWebIDBKeyPathTypeString:
      return KEY_PATH_TYPE_STRING;
    case blink::kWebIDBKeyPathTypeArray:
      return KEY_PATH_TYPE_ARRAY;
  }
  NOTREACHED();
  return KEY_PATH_TYPE_NONE;
}
