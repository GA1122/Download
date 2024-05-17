void BackendIO::OnDone(bool cancel) {
  if (IsEntryOperation()) {
    CACHE_UMA(TIMES, "TotalIOTime", 0, ElapsedTime());
  }

  if (!ReturnsEntry())
    return;

  if (result() == net::OK) {
    static_cast<EntryImpl*>(*entry_ptr_)->OnEntryCreated(backend_);
    if (cancel)
      (*entry_ptr_)->Close();
  }
}
