  void OnAudioTimeCallback(
      base::TimeDelta current_time, base::TimeDelta max_time) {
    CHECK(current_time <= max_time);
  }
