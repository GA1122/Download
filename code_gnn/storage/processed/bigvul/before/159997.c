bool DiskCacheBackendTest::EnumerateAndMatchKeys(
    int max_to_open,
    TestIterator* iter,
    std::set<std::string>* keys_to_match,
    size_t* count) {
  disk_cache::Entry* entry;

  if (!iter)
    return false;
  while (iter->OpenNextEntry(&entry) == net::OK) {
    if (!entry)
      return false;
    EXPECT_EQ(1U, keys_to_match->erase(entry->GetKey()));
    entry->Close();
    ++(*count);
    if (max_to_open >= 0 && static_cast<int>(*count) >= max_to_open)
      break;
  };

  return true;
}
