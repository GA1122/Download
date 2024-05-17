 virtual int64_t getTimeUsOfGranule(uint64_t granulePos) const {
 if (granulePos > INT64_MAX / 1000000ll) {
 return INT64_MAX;
 }
 return granulePos * 1000000ll / mVi.rate;
 }
