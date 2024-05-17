  const MockAutoFillMetrics* metric_logger() const {
    return static_cast<const MockAutoFillMetrics*>(
        AutoFillManager::metric_logger());
  }
