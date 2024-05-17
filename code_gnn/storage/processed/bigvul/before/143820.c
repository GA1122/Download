  std::unique_ptr<SparseHistogram> NewSparseHistogram(const char* name) {
    return std::unique_ptr<SparseHistogram>(new SparseHistogram(name));
  }
