bool Contains(const Collection& collection, const Key& key) {
  return std::find(collection.begin(), collection.end(), key) !=
      collection.end();
}
