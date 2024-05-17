void BackgroundLoaderOffliner::AddLoadingSignal(const char* signal_name) {
  base::TimeTicks current_time = base::TimeTicks::Now();
  base::TimeDelta delay_so_far = current_time - load_start_time_;
  double delay = delay_so_far.InMilliseconds();
  signal_data_.SetDouble(signal_name, delay);
}
