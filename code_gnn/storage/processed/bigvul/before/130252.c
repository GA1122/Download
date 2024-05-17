FormatEtcEnumerator::FormatEtcEnumerator(
    DataObjectImpl::StoredData::const_iterator start,
    DataObjectImpl::StoredData::const_iterator end)
    : ref_count_(0), cursor_(0) {
  while (start != end) {
    FORMATETC* format_etc = new FORMATETC;
    CloneFormatEtc(&(*start)->format_etc, format_etc);
    contents_.push_back(format_etc);
    ++start;
  }
}
