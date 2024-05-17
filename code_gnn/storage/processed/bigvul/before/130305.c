STDMETHODIMP FormatEtcEnumerator::Skip(ULONG skip_count) {
  cursor_ += skip_count;
  return cursor_ <= contents_.size() ? S_OK : S_FALSE;
}
