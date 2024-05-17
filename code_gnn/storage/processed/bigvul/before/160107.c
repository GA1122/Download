void BackendImpl::TrimDeletedListForTest(bool empty) {
  eviction_.SetTestMode();
  eviction_.TrimDeletedList(empty);
}
