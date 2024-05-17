bool AudioHandler::PropagatesSilence() const {
  return last_non_silent_time_ + LatencyTime() + TailTime() <
         Context()->currentTime();
}
