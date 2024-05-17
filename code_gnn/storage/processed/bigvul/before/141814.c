void MetricsLog::CloseLog() {
  DCHECK(!closed_);
  closed_ = true;
}
