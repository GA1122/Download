int Document::ElapsedTime() const {
  return static_cast<int>((CurrentTime() - start_time_) * 1000);
}
