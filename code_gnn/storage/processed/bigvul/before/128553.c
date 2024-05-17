void Instance::DocumentLoadProgress(uint32 available, uint32 doc_size) {
  double progress = 0.0;
  if (doc_size == 0) {
    static const double kFactor = log(100000000.0) / 100.0;
    if (available > 0) {
      progress = log(static_cast<double>(available)) / kFactor;
      if (progress > 100.0)
        progress = 100.0;
    }
  } else {
    progress = 100.0 * static_cast<double>(available) / doc_size;
  }
  progress_bar_.SetProgress(progress);
}
