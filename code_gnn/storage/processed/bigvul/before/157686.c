int64_t GenerateSequenceNumber() {
  static int64_t next_number = base::Time::Now().ToDoubleT() * 1000000;
  return ++next_number;
}
