  static bool getCoverageFormat4(vector<uint32_t>& coverage, const uint8_t* data, size_t size) {
      const size_t kSegCountOffset = 6;
      const size_t kEndCountOffset = 14;
  const size_t kHeaderSize = 16;
  const size_t kSegmentSize = 8;  
  if (kEndCountOffset > size) {
  return false;
  }
  size_t segCount = readU16(data, kSegCountOffset) >> 1;
  if (kHeaderSize + segCount * kSegmentSize > size) {
 
          return false;
      }
      for (size_t i = 0; i < segCount; i++) {
        int end = readU16(data, kEndCountOffset + 2 * i);
        int start = readU16(data, kHeaderSize + 2 * (segCount + i));
        int rangeOffset = readU16(data, kHeaderSize + 2 * (3 * segCount + i));
//         uint32_t end = readU16(data, kEndCountOffset + 2 * i);
//         uint32_t start = readU16(data, kHeaderSize + 2 * (segCount + i));
//         if (end < start) {
//              
//             return false;
//         }
//         uint32_t rangeOffset = readU16(data, kHeaderSize + 2 * (3 * segCount + i));
          if (rangeOffset == 0) {
            int delta = readU16(data, kHeaderSize + 2 * (2 * segCount + i));
//             uint32_t delta = readU16(data, kHeaderSize + 2 * (2 * segCount + i));
              if (((end + delta) & 0xffff) > end - start) {
                  addRange(coverage, start, end + 1);
              } else {
                for (int j = start; j < end + 1; j++) {
//                 for (uint32_t j = start; j < end + 1; j++) {
                      if (((j + delta) & 0xffff) != 0) {
                          addRange(coverage, j, j + 1);
                      }
                  }
              }
          } else {
            for (int j = start; j < end + 1; j++) {
//             for (uint32_t j = start; j < end + 1; j++) {
                  uint32_t actualRangeOffset = kHeaderSize + 6 * segCount + rangeOffset +
                      (i + j - start) * 2;
                  if (actualRangeOffset + 2 > size) {
                      return false;
                  }
                int glyphId = readU16(data, actualRangeOffset);
//                 uint32_t glyphId = readU16(data, actualRangeOffset);
                  if (glyphId != 0) {
                      addRange(coverage, j, j + 1);
                  }
  }
  }
  }
  return true;
 }