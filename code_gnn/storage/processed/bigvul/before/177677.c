   static void SetUpTestCase() {
     src_ = reinterpret_cast<uint8_t*>(vpx_memalign(kDataAlignment, kSrcSize));
    dst_ = reinterpret_cast<uint8_t*>(vpx_memalign(kDataAlignment, kDstSize));
    dst_c_ = reinterpret_cast<uint8_t*>(vpx_memalign(kDataAlignment, kDstSize));
 }
