bool MockNetworkTransaction::GetLoadTimingInfo(
    LoadTimingInfo* load_timing_info) const {
  if (socket_log_id_ != NetLog::Source::kInvalidId) {
    load_timing_info->socket_reused = false;
    load_timing_info->socket_log_id = socket_log_id_;
    load_timing_info->connect_timing.connect_start = base::TimeTicks::Now();
    load_timing_info->connect_timing.connect_end = base::TimeTicks::Now();
    load_timing_info->send_start = base::TimeTicks::Now();
    load_timing_info->send_end = base::TimeTicks::Now();
  } else {
    load_timing_info->socket_reused = true;
    load_timing_info->send_start = base::TimeTicks::Now();
    load_timing_info->send_end = base::TimeTicks::Now();
  }
  return true;
}
