  void CreateGlobalHistograms(int histogram_count) {
    DCHECK_GT(kMaxCreateHistograms, histogram_count);

    created_histograms_[0] = base::SparseHistogram::FactoryGet("h0", 0);
    created_histograms_[0]->Add(0);
    for (int i = 1; i < histogram_count; ++i) {
      created_histograms_[i] = base::Histogram::FactoryGet(
          base::StringPrintf("h%d", i), 1, 100, 10, 0);
      created_histograms_[i]->Add(i);
    }
  }
