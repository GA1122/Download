  FileMetricsProvider::FilterAction FilterSourcePath(
      const base::FilePath& path) {
    DCHECK_LT(0U, filter_actions_remaining_);
    --filter_actions_remaining_;
    return *filter_actions_++;
  }
