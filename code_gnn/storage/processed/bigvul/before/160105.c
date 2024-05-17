int BackendImpl::SyncOpenNextEntry(Rankings::Iterator* iterator,
                                   scoped_refptr<EntryImpl>* next_entry) {
  *next_entry = OpenNextEntryImpl(iterator);
  return (*next_entry) ? net::OK : net::ERR_FAILED;
}
