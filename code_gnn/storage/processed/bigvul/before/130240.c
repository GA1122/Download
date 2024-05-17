FormatEtcEnumerator* FormatEtcEnumerator::CloneFromOther(
    const FormatEtcEnumerator* other) {
  FormatEtcEnumerator* e = new FormatEtcEnumerator;
  ScopedVector<FORMATETC>::const_iterator start = other->contents_.begin();
  while (start != other->contents_.end()) {
    FORMATETC* format_etc = new FORMATETC;
    CloneFormatEtc(*start, format_etc);
    e->contents_.push_back(format_etc);
    ++start;
  }
  e->cursor_ = other->cursor_;
  return e;
}
