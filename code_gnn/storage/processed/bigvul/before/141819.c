MetricsLog::IndependentMetricsLoader::IndependentMetricsLoader(
    std::unique_ptr<MetricsLog> log)
    : log_(std::move(log)),
      flattener_(new IndependentFlattener(log_.get())),
      snapshot_manager_(new base::HistogramSnapshotManager(flattener_.get())) {}
