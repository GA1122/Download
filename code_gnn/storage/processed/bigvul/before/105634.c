 std::string GenerateRandomID() {
   uint64 msb = base::RandUint64();
   uint64 lsb = base::RandUint64();
  return base::StringPrintf("%016" PRIx64 "%016" PRIx64, msb, lsb);
}
