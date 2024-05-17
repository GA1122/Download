bool BackendImpl::CheckEntry(EntryImpl* cache_entry) {
  bool ok = block_files_.IsValid(cache_entry->entry()->address());
  ok = ok && block_files_.IsValid(cache_entry->rankings()->address());
  EntryStore* data = cache_entry->entry()->Data();
  for (size_t i = 0; i < arraysize(data->data_addr); i++) {
    if (data->data_addr[i]) {
      Addr address(data->data_addr[i]);
      if (address.is_block_file())
        ok = ok && block_files_.IsValid(address);
    }
  }

  return ok && cache_entry->rankings()->VerifyHash();
}
