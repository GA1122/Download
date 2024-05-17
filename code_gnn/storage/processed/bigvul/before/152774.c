bool SparseHistogram::SerializeInfoImpl(Pickle* pickle) const {
  return pickle->WriteString(histogram_name()) && pickle->WriteInt(flags());
}
