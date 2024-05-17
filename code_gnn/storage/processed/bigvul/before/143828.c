  void OnHistogramChanged(base::HistogramBase::Sample histogram_value) {
    called = true;
    last_histogram_value = histogram_value;
  }
