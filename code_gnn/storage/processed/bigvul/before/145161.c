void GpuProcessHost::IncrementCrashCount(int forgive_minutes,
                                         int* crash_count) {
  DCHECK_GT(forgive_minutes, 0);

  static base::TimeTicks last_crash_time;

  base::TimeTicks current_time = base::TimeTicks::Now();
  if (crashed_before_) {
    int minutes_delta = (current_time - last_crash_time).InMinutes();
    int crashes_to_forgive = minutes_delta / forgive_minutes;
    *crash_count = std::max(0, *crash_count - crashes_to_forgive);
  }
  ++(*crash_count);

  crashed_before_ = true;
  last_crash_time = current_time;
}
