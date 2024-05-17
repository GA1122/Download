void RecordOpen(const base::Time& end) {
  if (!end.is_null())
    UMA_HISTOGRAM_LONG_TIMES("Download.OpenTime", (base::Time::Now() - end));
}
