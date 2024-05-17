  bool ProvideIndependentMetrics(
      SystemProfileProto* profile_proto,
      base::HistogramSnapshotManager* snapshot_manager) {
    return provider()->ProvideIndependentMetrics(profile_proto,
                                                 snapshot_manager);
  }
