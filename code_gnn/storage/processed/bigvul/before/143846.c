  base::HistogramBase* GetCreatedHistogram(int index) {
    DCHECK_GT(kMaxCreateHistograms, index);
    return created_histograms_[index];
  }
