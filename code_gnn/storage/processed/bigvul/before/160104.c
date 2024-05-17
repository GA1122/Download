int BackendImpl::SyncOpenEntry(const std::string& key,
                               scoped_refptr<EntryImpl>* entry) {
  DCHECK(entry);
  *entry = OpenEntryImpl(key);
  return (*entry) ? net::OK : net::ERR_FAILED;
}
