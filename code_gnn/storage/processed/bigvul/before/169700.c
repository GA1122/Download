  bool UpdateAndCheckUniqueness(v8::Local<v8::Object> handle) {
    typedef HashToHandleMap::const_iterator Iterator;
    int hash = avoid_identity_hash_for_testing_ ? 0 : handle->GetIdentityHash();
    std::pair<Iterator, Iterator> range = unique_map_.equal_range(hash);
    for (Iterator it = range.first; it != range.second; ++it) {
      if (it->second == handle)
        return false;
    }
    unique_map_.insert(std::make_pair(hash, handle));
    return true;
  }
