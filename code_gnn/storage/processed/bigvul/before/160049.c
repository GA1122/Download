void BackendImpl::IncreaseNumEntries() {
  data_->header.num_entries++;
  DCHECK_GT(data_->header.num_entries, 0);
}
