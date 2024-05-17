int BackendImpl::SyncCreateEntry(const std::string& key,
                                 scoped_refptr<EntryImpl>* entry) {
  DCHECK(entry);
  *entry = CreateEntryImpl(key);
  return (*entry) ? net::OK : net::ERR_FAILED;
}
