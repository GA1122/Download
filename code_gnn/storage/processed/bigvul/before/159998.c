int DiskCacheBackendTest::GeneratePendingIO(net::TestCompletionCallback* cb) {
  if (!use_current_thread_ && !simple_cache_mode_) {
    ADD_FAILURE();
    return net::ERR_FAILED;
  }

  disk_cache::Entry* entry;
  int rv = cache_->CreateEntry("some key", &entry, cb->callback());
  if (cb->GetResult(rv) != net::OK)
    return net::ERR_CACHE_CREATE_FAILURE;

  const int kSize = 25000;
  scoped_refptr<net::IOBuffer> buffer(new net::IOBuffer(kSize));
  CacheTestFillBuffer(buffer->data(), kSize, false);

  for (int i = 0; i < 10 * 1024 * 1024; i += 64 * 1024) {
    if (!simple_cache_mode_) {
      rv = static_cast<disk_cache::EntryImpl*>(entry)->WriteDataImpl(
          0, i, buffer.get(), kSize, cb->callback(), false);
    } else {
      rv = entry->WriteData(0, i, buffer.get(), kSize, cb->callback(), false);
    }

    if (rv == net::ERR_IO_PENDING)
      break;
    if (rv != kSize)
      rv = net::ERR_FAILED;
  }

  if (!simple_cache_mode_)
    static_cast<disk_cache::EntryImpl*>(entry)->Release();
  else
    entry->Close();

  return rv;
}
