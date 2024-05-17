void BackendImpl::NotLinked(EntryImpl* entry) {
  Addr entry_addr = entry->entry()->address();
  uint32_t i = entry->GetHash() & mask_;
  Addr address(data_->table[i]);
  if (!address.is_initialized())
    return;

  for (;;) {
    DCHECK(entry_addr.value() != address.value());
    address.set_value(GetNextAddr(address));
    if (!address.is_initialized())
      break;
  }
}
