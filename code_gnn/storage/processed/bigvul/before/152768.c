HistogramBase* SparseHistogram::DeserializeInfoImpl(PickleIterator* iter) {
  std::string histogram_name;
  int flags;
  if (!iter->ReadString(&histogram_name) || !iter->ReadInt(&flags)) {
    DLOG(ERROR) << "Pickle error decoding Histogram: " << histogram_name;
    return NULL;
  }

  flags &= ~HistogramBase::kIPCSerializationSourceFlag;

  return SparseHistogram::FactoryGet(histogram_name, flags);
}
