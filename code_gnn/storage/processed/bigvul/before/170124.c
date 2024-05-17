bool ContainsNonNullEntryForNonNullKey(
    const std::map<Key*, linked_ptr<Value>>& map,
    Key* key) {
  if (!key)
    return false;
  auto it = map.find(key);
  return it != map.end() && it->second.get();
}
