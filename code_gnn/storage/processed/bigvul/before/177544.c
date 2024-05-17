 int BorderLeft() const {
 const int center = (kOuterBlockSize - Width()) / 2;
 return (center + (kDataAlignment - 1)) & ~(kDataAlignment - 1);
 }
