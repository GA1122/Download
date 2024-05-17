static void AddHistogramSample(void* hist, int sample) {
  Histogram* histogram = static_cast<Histogram *>(hist);
  histogram->Add(sample);
}
