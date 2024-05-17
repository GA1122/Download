    GetEstimatedMemoryFreedOnDiscardKB() const {
#if defined(OS_CHROMEOS)
  std::unique_ptr<base::ProcessMetrics> process_metrics(
      base::ProcessMetrics::CreateProcessMetrics(GetProcessHandle()));
  base::ProcessMetrics::TotalsSummary summary =
      process_metrics->GetTotalsSummary();
  return summary.private_clean_kb + summary.private_dirty_kb + summary.swap_kb;
#else
  return 0;
#endif
}
