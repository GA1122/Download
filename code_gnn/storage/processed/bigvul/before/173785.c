static uint64_t ns_to_samples(uint64_t ns, int kRate) {
 return static_cast<double>(ns) * kRate / 1000000000;
}
