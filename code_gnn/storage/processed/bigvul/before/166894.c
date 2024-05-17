DOMHighResTimeStamp Performance::MonotonicTimeToDOMHighResTimeStamp(
    TimeTicks monotonic_time) const {
  return MonotonicTimeToDOMHighResTimeStamp(time_origin_, monotonic_time,
                                            false  );
}
