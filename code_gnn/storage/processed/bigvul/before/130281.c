STDMETHODIMP FormatEtcEnumerator::Next(
    ULONG count, FORMATETC* elements_array, ULONG* elements_fetched) {
  if (!elements_fetched)
    DCHECK_EQ(count, 1ul);

  ULONG index = 0;
  while (cursor_ < contents_.size() && index < count) {
    CloneFormatEtc(contents_[cursor_], &elements_array[index]);
    ++cursor_;
    ++index;
  }
  if (elements_fetched)
    *elements_fetched = index;

  return index == count ? S_OK : S_FALSE;
}
